#include "mcs_sync.h"

/*
 * mcs_lockdep_init — reset semua field ke nol/kosong
 */
void mcs_lockdep_init(mcs_lockdep_state_t *st) {
    if (st == 0) {
        return;
    }
    for (uint32_t i = 0u; i < MCS_LOCKDEP_MAX_HELD; i++) {
        st->held_class[i] = 0u;
        st->held_name[i]  = 0;
    }
    st->depth           = 0u;
    st->violation_count = 0u;
}

/*
 * mcs_lockdep_before_acquire — validasi sebelum lock diambil
 * Menolak:
 *   1. Null pointer             -> MCS_SYNC_EINVAL
 *   2. Stack penuh              -> MCS_SYNC_EOVERFLOW
 *   3. class_id sama (rekursi)  -> MCS_SYNC_EDEADLK
 *   4. class_id < top of stack  -> MCS_SYNC_EDEADLK (order inversion)
 */
int mcs_lockdep_before_acquire(mcs_lockdep_state_t *st,
                                uint32_t class_id,
                                const char *name) {
    if (st == 0) {
        return MCS_SYNC_EINVAL;
    }
    if (st->depth >= MCS_LOCKDEP_MAX_HELD) {
        st->violation_count++;
        return MCS_SYNC_EOVERFLOW;
    }

    /* Cek rekursi: class_id yang sama sudah ada di stack */
    for (uint32_t i = 0u; i < st->depth; i++) {
        if (st->held_class[i] == class_id) {
            st->violation_count++;
            return MCS_SYNC_EDEADLK;
        }
    }

    /* Cek order inversion: class_id harus > semua yang sudah dipegang */
    if (st->depth > 0u) {
        uint32_t top = st->held_class[st->depth - 1u];
        if (class_id <= top) {
            st->violation_count++;
            return MCS_SYNC_EDEADLK;
        }
    }

    /* Aman: push ke stack */
    st->held_class[st->depth] = class_id;
    st->held_name[st->depth]  = name;
    st->depth++;
    return MCS_SYNC_OK;
}

/*
 * mcs_lockdep_after_release — validasi pelepasan lock (harus LIFO)
 * Menolak:
 *   1. Null pointer             -> MCS_SYNC_EINVAL
 *   2. Stack kosong             -> MCS_SYNC_EINVAL
 *   3. class_id bukan top       -> MCS_SYNC_EPERM (non-LIFO)
 */
int mcs_lockdep_after_release(mcs_lockdep_state_t *st,
                               uint32_t class_id,
                               const char *name) {
    (void)name;
    if (st == 0) {
        return MCS_SYNC_EINVAL;
    }
    if (st->depth == 0u) {
        return MCS_SYNC_EINVAL;
    }
    uint32_t top = st->held_class[st->depth - 1u];
    if (top != class_id) {
        st->violation_count++;
        return MCS_SYNC_EPERM;
    }
    st->depth--;
    st->held_class[st->depth] = 0u;
    st->held_name[st->depth]  = 0;
    return MCS_SYNC_OK;
}
