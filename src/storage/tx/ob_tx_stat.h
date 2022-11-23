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

#ifndef OCEANBASE_TRANSACTION_OB_TX_STAT_
#define OCEANBASE_TRANSACTION_OB_TX_STAT_

#include "ob_trans_define.h"
#include "common/ob_range.h"

namespace oceanbase
{
namespace transaction
{

struct ObTxStat
{
  ObTxStat() { reset(); }
  ~ObTxStat() { }
  void reset();
  int init(const common::ObAddr &addr, const ObTransID &tx_id,
           const uint64_t tenant_id,  const bool has_decided,
           const share::ObLSID &ls_id, const share::ObLSArray &participants,
           const int64_t tx_ctx_create_time, const int64_t tx_expired_time,
           const int64_t ref_cnt, const int64_t last_op_sn,
           const int64_t pending_write, const int64_t state,
           const int tx_type, const int64_t part_tx_action,
           const void* const tx_ctx_addr,
           const int64_t pending_log_size, const int64_t flushed_log_size,
           const int64_t role_state,
           const int64_t session_id, const common::ObAddr &scheduler,
           const bool is_exiting);
  TO_STRING_KV(K_(addr), K_(tx_id), K_(tenant_id),
      K_(has_decided), K_(ls_id), K_(participants),
      K_(tx_ctx_create_time), K_(tx_expired_time), K_(ref_cnt),
      K_(last_op_sn), K_(pending_write), K_(state), K_(tx_type),
      KP_(tx_ctx_addr),
      K_(pending_log_size), K_(flushed_log_size),
      K_(role_state), K_(session_id),
      K_(scheduler_addr),
      K_(is_exiting));
public:
  bool is_inited_;
  common::ObAddr addr_;
  ObTransID tx_id_;
  uint64_t tenant_id_;
  bool has_decided_;
  share::ObLSID ls_id_;
  share::ObLSArray participants_;
  int64_t tx_ctx_create_time_;
  int64_t tx_expired_time_;
  int64_t ref_cnt_;
  int64_t last_op_sn_;
  int64_t pending_write_;
  int64_t state_;
  int tx_type_;
  int64_t part_tx_action_;
  const void *tx_ctx_addr_;
  int64_t pending_log_size_;
  int64_t flushed_log_size_;
  int64_t role_state_;
  int64_t session_id_;
  common::ObAddr scheduler_addr_;
  bool is_exiting_;
};

class ObTxLockStat
{
public:
  ObTxLockStat() { reset(); }
  ~ObTxLockStat() {}
  int init(const common::ObAddr &addr,
            uint64_t tenant_id,
            const share::ObLSID &ls_id,
            const ObMemtableKeyInfo &memtable_key_info,
            uint32_t session_id,
            uint64_t proxy_session_id,
            const ObTransID &tx_id,
            int64_t tx_ctx_create_time,
            int64_t tx_expired_time);
  void reset();
  const common::ObAddr &get_addr() const { return addr_; }
  uint64_t get_tenant_id() const { return tenant_id_; }
  const share::ObLSID &get_ls_id() const { return ls_id_; }
  const ObMemtableKeyInfo &get_memtable_key_info() const { return memtable_key_info_; }
  uint32_t get_session_id() const { return session_id_; }
  uint64_t get_proxy_session_id() const { return proxy_session_id_; }
  const ObTransID &get_tx_id() const { return tx_id_; }
  int64_t get_tx_ctx_create_time() const { return tx_ctx_create_time_; }
  int64_t get_tx_expired_time() const { return tx_expired_time_; }

  TO_STRING_KV(K_(addr),
               K_(tenant_id),
               K_(ls_id),
               K_(memtable_key_info),
               K_(session_id),
               K_(proxy_session_id),
               K_(tx_id),
               K_(tx_ctx_create_time),
               K_(tx_expired_time));

private:
  bool is_inited_;
  common::ObAddr addr_;
  uint64_t tenant_id_;
  share::ObLSID ls_id_;
  ObMemtableKeyInfo memtable_key_info_;
  uint32_t session_id_;
  uint64_t proxy_session_id_;
  ObTransID tx_id_;
  int64_t tx_ctx_create_time_;
  int64_t tx_expired_time_;
};

} // transaction
} // oceanbase
#endif // OCEANABAE_TRANSACTION_OB_TX_STAT_
