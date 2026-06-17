#include "mcs_sync.h"

/*
 * mcs_mutex_init — inisialisasi mutex ke state unlocked, owner = 0
 */
void mcs_mutex_init(mcs_mutex_t *mutex, uint32_t class_id, const char *name) {
    if (mutex == 0) {
        return;
    }
    __atomic_store_n(&mutex->locked, 0u, __ATOMIC_RELAXED);
    __atomic_store_n(&mutex->owner,  0u, __ATOMIC_RELAXED);
    mutex->class_id = class_id;
    mutex->name     = name;
}

/*
 * mcs_mutex_try_lock — satu kali percobaan lock dengan owner_id
 * Menolak:
 *   - owner_id == 0              -> MCS_SYNC_EINVAL
 *   - mutex sudah locked oleh owner yang sama -> MCS_SYNC_EDEADLK (rekursi)
 *   - mutex sudah locked oleh owner lain      -> MCS_SYNC_EBUSY
 */
int mcs_mutex_try_lock(mcs_mutex_t *mutex, uint64_t owner_id) {
    if (mutex == 0 || owner_id == 0u) {
        return MCS_SYNC_EINVAL;
    }

    uint32_t expected = 0u;
    uint32_t desired  = 1u;
    int ok = __atomic_compare_exchange_n(
        &mutex->locked,
        &expected,
        desired,
        0,
        __ATOMIC_ACQUIRE,
        __ATOMIC_RELAXED
    );

    if (ok) {
        /* Berhasil: catat owner */
        __atomic_store_n(&mutex->owner, owner_id, __ATOMIC_RELAXED);
        return MCS_SYNC_OK;
    }

    /* Lock sedang dipegang — cek apakah oleh owner yang sama */
    uint64_t current_owner = __atomic_load_n(&mutex->owner, __ATOMIC_RELAXED);
    if (current_owner == owner_id) {
        /* Rekursi: tolak */
        return MCS_SYNC_EDEADLK;
    }

    /* Dipegang owner lain */
    return MCS_SYNC_EBUSY;
}

/*
 * mcs_mutex_unlock — lepas mutex, hanya boleh dilakukan oleh owner
 */
int mcs_mutex_unlock(mcs_mutex_t *mutex, uint64_t owner_id) {
    if (mutex == 0 || owner_id == 0u) {
        return MCS_SYNC_EINVAL;
    }
    if (__atomic_load_n(&mutex->locked, __ATOMIC_RELAXED) == 0u) {
        return MCS_SYNC_EINVAL;
    }
    uint64_t current_owner = __atomic_load_n(&mutex->owner, __ATOMIC_RELAXED);
    if (current_owner != owner_id) {
        return MCS_SYNC_EPERM;
    }
    /* Hapus owner sebelum release lock */
    __atomic_store_n(&mutex->owner,  0u, __ATOMIC_RELEASE);
    __atomic_store_n(&mutex->locked, 0u, __ATOMIC_RELEASE);
    return MCS_SYNC_OK;
}

/*
 * mcs_mutex_is_locked — baca state mutex (relaxed, hanya diagnostik)
 */
bool mcs_mutex_is_locked(const mcs_mutex_t *mutex) {
    if (mutex == 0) {
        return false;
    }
    return __atomic_load_n(&mutex->locked, __ATOMIC_RELAXED) != 0u;
}

/*
 * mcs_mutex_owner — baca owner saat ini (relaxed, hanya diagnostik)
 */
uint64_t mcs_mutex_owner(const mcs_mutex_t *mutex) {
    if (mutex == 0) {
        return 0u;
    }
    return __atomic_load_n(&mutex->owner, __ATOMIC_RELAXED);
}
