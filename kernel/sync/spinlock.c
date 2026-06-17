#include "mcs_sync.h"

/*
 * mcs_spin_init — inisialisasi spinlock ke state unlocked
 */
void mcs_spin_init(mcs_spinlock_t *lock, uint32_t class_id, const char *name) {
    if (lock == 0) {
        return;
    }
    __atomic_store_n(&lock->locked, 0u, __ATOMIC_RELAXED);
    lock->class_id = class_id;
    lock->name     = name;
}

/*
 * mcs_spin_try_lock — satu kali percobaan acquire
 * Return MCS_SYNC_OK jika berhasil, MCS_SYNC_EBUSY jika sudah terkunci
 */
int mcs_spin_try_lock(mcs_spinlock_t *lock) {
    if (lock == 0) {
        return MCS_SYNC_EINVAL;
    }
    uint32_t expected = 0u;
    uint32_t desired  = 1u;
    int ok = __atomic_compare_exchange_n(
        &lock->locked,
        &expected,
        desired,
        0,                   /* strong */
        __ATOMIC_ACQUIRE,
        __ATOMIC_RELAXED
    );
    return ok ? MCS_SYNC_OK : MCS_SYNC_EBUSY;
}

/*
 * mcs_spin_lock — busy-wait hingga lock berhasil diambil
 */
void mcs_spin_lock(mcs_spinlock_t *lock) {
    if (lock == 0) {
        return;
    }
    while (__atomic_exchange_n(&lock->locked, 1u, __ATOMIC_ACQUIRE) != 0u) {
        /* busy wait: spin sampai lock bebas */
        while (__atomic_load_n(&lock->locked, __ATOMIC_RELAXED) != 0u) {
            /* pause hint — pada x86_64 ini menghasilkan REP NOP / PAUSE */
            __asm__ volatile("pause" ::: "memory");
        }
    }
}

/*
 * mcs_spin_unlock — lepas lock dengan release ordering
 */
void mcs_spin_unlock(mcs_spinlock_t *lock) {
    if (lock == 0) {
        return;
    }
    __atomic_store_n(&lock->locked, 0u, __ATOMIC_RELEASE);
}

/*
 * mcs_spin_is_locked — baca state lock (relaxed, hanya untuk diagnostik)
 */
bool mcs_spin_is_locked(const mcs_spinlock_t *lock) {
    if (lock == 0) {
        return false;
    }
    return __atomic_load_n(&lock->locked, __ATOMIC_RELAXED) != 0u;
}
