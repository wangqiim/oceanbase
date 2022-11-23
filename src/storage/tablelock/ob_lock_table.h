/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#ifndef OCEANBASE_STORAGE_OB_LOCK_TABLE_H_
#define OCEANBASE_STORAGE_OB_LOCK_TABLE_H_
#include <stdint.h>
#include "lib/lock/ob_spin_lock.h"
#include "lib/worker.h"
#include "storage/ob_i_table.h"
#include "storage/tablelock/ob_obj_lock.h"

namespace oceanbase
{
namespace obrpc
{
class ObBatchCreateTabletArg;
class ObBatchRemoveTabletArg;
}

namespace common
{
class ObTabletID;
}

namespace share
{
class ObLSID;
namespace schema
{
class ObTableSchema;
}
}

namespace storage
{
class ObLS;
struct ObStoreCtx;
class ObITable;
class ObStoreRow;
class ObStoreCtx;
}

namespace memtable
{
class ObMemtableCtx;
}

namespace transaction
{
namespace tablelock
{
struct ObLockParam;
class ObTableLockOp;
class ObLockMemtable;

class ObLockTable
{
public:
  ObLockTable()
    : parent_(nullptr),
      is_inited_(false)
  {}
  ~ObLockTable() {}
  int init(storage::ObLS *parent);
  int prepare_for_safe_destroy();
  void destroy();
  int offline();
  int online();
  // create lock table tablet.
  int create_tablet(const lib::Worker::CompatMode compat_mode, const int64_t create_scn);
  // remove lock table tablet.
  int remove_tablet();
  // load lock for tablet.
  int load_lock();
  int get_lock_memtable(storage::ObTableHandleV2 &handle);
  // check whether the lock op is conflict with exist dml lock.
  // @param[in] ctx, the store ctx of current transaction.
  // @param[in] param, contain the parameters need.
  int check_lock_conflict(storage::ObStoreCtx &ctx,
                          const ObLockParam &param);
  // check whether the lock op is conflict with exist lock.
  // @param[in] mem_ctx, the memtable ctx of current transaction.
  // @param[in] lock_op, which lock op will try to execute.
  // @param[in] include_finish_tx, whether include the finished transaction.
  // @param[out] conflict_tx_set, contain the conflict transaction it.
  int check_lock_conflict(
      const memtable::ObMemtableCtx *mem_ctx,
      const ObTableLockOp &lock_op,
      ObTxIDSet &conflict_tx_set,
      const bool include_finish_tx = true);
  // lock a object
  // @param[in] ctx, store ctx for trans.
  // @param[in] param, contain the lock id, lock type and so on.
  int lock(storage::ObStoreCtx &ctx,
           const ObLockParam &param);
  // unlock a object
  // @param[in] ctx, store ctx for trans.
  // @param[in] unlock_op, contain the lock id, lock type and so on.
  int unlock(storage::ObStoreCtx &ctx,
             const ObLockParam &param);
  // get all the lock id in the lock map
  // @param[out] iter, the iterator returned.
  // int get_lock_id_iter(ObLockIDIterator &iter);
  int get_lock_id_iter(ObLockIDIterator &iter);

  // get the lock op iterator of a obj lock
  // @param[in] lock_id, which obj lock's lock op will be iterated.
  // @param[out] iter, the iterator returned.
  int get_lock_op_iter(const ObLockID &lock_id,
                       ObLockOpIterator &iter);
private:
  // We use the method to recover the lock_table for reboot.
  int restore_lock_table_(storage::ObITable &sstable);
  int recover_(const blocksstable::ObDatumRow &row);
  int get_table_schema_(const uint64_t tenant_id,
                        share::schema::ObTableSchema &schema);
  int gen_create_tablet_arg_(const common::ObTabletID &tablet_id,
                             const uint64_t tenant_id,
                             const share::ObLSID ls_id,
                             const lib::Worker::CompatMode compat_mode,
                             const share::schema::ObTableSchema &table_schema,
                             obrpc::ObBatchCreateTabletArg &arg);
  int gen_remove_tablet_arg_(const common::ObTabletID &tablet_id,
                             const uint64_t tenant_id,
                             const share::ObLSID ls_id,
                             obrpc::ObBatchRemoveTabletArg &arg);

private:
  static const int64_t LOCKTABLE_SCHEMA_VERSION = 0;
  static const int64_t LOCKTABLE_SCHEMA_ROEKEY_CNT = 1;
  static const int64_t LOCKTABLE_SCHEMA_COLUMN_CNT = 2;
  storage::ObLS *parent_;
  bool is_inited_;
};

} // tablelock
} // transaction
} // oceanbase


#endif
