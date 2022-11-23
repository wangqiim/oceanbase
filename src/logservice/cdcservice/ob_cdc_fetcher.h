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

#ifndef OCEANBASE_LOGSERVICE_OB_CDC_FETCHER_
#define OCEANBASE_LOGSERVICE_OB_CDC_FETCHER_

#include "share/ob_ls_id.h"                     // ObLSID
#include "logservice/palf/lsn.h"                // LSN
#include "logservice/palf/log_group_entry.h"    // LogGroupEntry
#include "logservice/palf/log_entry.h"          // LogEntry
#include "logservice/palf/palf_iterator.h"      // PalfGroupBufferIterator
#include "logservice/palf_handle_guard.h"       // PalfHandleGuard
#include "ob_cdc_req.h"                         // RPC Request and Response
#include "ob_cdc_define.h"

namespace oceanbase
{
namespace cdc
{
using oceanbase::storage::ObLSService;
using oceanbase::share::ObLSID;
using oceanbase::palf::LSN;
using oceanbase::palf::LogGroupEntry;
using oceanbase::palf::LogEntry;

struct FetchRunTime;

class ObCdcFetcher
{
  // When fetch log finds that the remaining time is less than RPC_QIT_RESERVED_TIME,
  // exit immediately to avoid timeout
  static const int64_t RPC_QIT_RESERVED_TIME = 5 * 1000 * 1000; // 5 second

public:
  ObCdcFetcher();
  ~ObCdcFetcher();
  int init(const uint64_t tenant_id,
      ObLSService *ls_service);
  void destroy();

public:
  // Fetch LogGroupEntry
  int fetch_log(const obrpc::ObCdcLSFetchLogReq &req,
      obrpc::ObCdcLSFetchLogResp &resp);

  // Fetch Missing LogEntry
  // TODO optimize fetch LogEntry(Random read)
  int fetch_missing_log(const obrpc::ObCdcLSFetchMissLogReq &req,
      obrpc::ObCdcLSFetchLogResp &resp);

private:
  // @retval OB_SUCCESS         Success
  // @retval OB_ENTRY_NOT_EXIST LS not exist in this server
  int init_palf_handle_guard_(const ObLSID &ls_id,
      palf::PalfHandleGuard &palf_handle_guard);

  // @retval OB_SUCCESS         Success
  // @retval OB_ENTRY_NOT_EXIST LS not exist in this server
  int init_group_iterator_(const ObLSID &ls_id,
      const LSN &start_lsn,
      palf::PalfHandleGuard &palf_handle_guard,
      palf::PalfGroupBufferIterator &group_iter);
  int do_fetch_log_(const obrpc::ObCdcLSFetchLogReq &req,
      palf::PalfHandleGuard &palf_handle_guard,
      palf::PalfGroupBufferIterator &group_iter,
      FetchRunTime &fetch_runtime,
      obrpc::ObCdcLSFetchLogResp &resp);
  int ls_fetch_log_(const ObLSID &ls_id,
      palf::PalfGroupBufferIterator &group_iter,
      const int64_t end_tstamp,
      obrpc::ObCdcLSFetchLogResp &resp,
      FetchRunTime &frt,
      bool &reach_upper_limit,
      bool &reach_max_lsn,
      int64_t &scan_round_count,
      int64_t &fetched_log_count);
  // Check whether has reached time limit
  inline bool is_time_up_(const int64_t log_count, const int64_t end_tstamp)
  {
    // every batch of logs, check whether has timed out
    static const int64_t CHECK_TIMEOUT_LOG_COUNT = 100;
    static const int64_t CHECK_TIMEOUT_LOG_INDEX = 10;
    bool bool_ret = false;

    if (((log_count % CHECK_TIMEOUT_LOG_COUNT) == CHECK_TIMEOUT_LOG_INDEX)) {
      int64_t cur_time = ObTimeUtility::current_time();
      bool_ret = cur_time > end_tstamp;
    }
    return bool_ret;
  }
  // Check the next LogGroupEntry to determine whether need to continue fetching logs.
  void check_next_group_entry_(const LSN &next_lsn,
      const LogGroupEntry &next_log_group_entry,
      const int64_t fetched_log_count,
      obrpc::ObCdcLSFetchLogResp &resp,
      FetchRunTime &frt,
      bool &reach_upper_limit);
  // Fill Group Log Entry directly into resp_buf.
  // TODO Consider compression and decryption
  int prefill_resp_with_group_entry_(const ObLSID &ls_id,
      const LSN &lsn,
      LogGroupEntry &log_group_entry,
      obrpc::ObCdcLSFetchLogResp &resp);
  void handle_when_buffer_full_(FetchRunTime &frt);
  // lsn of ls_id wantted does not exist on this server, feed this information back to CDC Connector,
  // CDC Connector needs to change search server.
  int handle_log_not_exist_(const ObLSID &ls_id,
      obrpc::ObCdcLSFetchLogResp &resp);
  // handle when has reached max lsn in this server
  void handle_when_reach_max_lsn_(const ObLSID &ls_id,
      palf::PalfHandleGuard &palf_handle_guard,
      const int64_t fetched_log_count,
      FetchRunTime &frt,
      obrpc::ObCdcLSFetchLogResp &resp);
  // Check whether is backward standby server
  int check_lag_follower_(const ObLSID &ls_id,
      palf::PalfHandleGuard &palf_handle_guard,
      obrpc::ObCdcLSFetchLogResp &resp);
  int do_fetch_missing_log_(const obrpc::ObCdcLSFetchMissLogReq &req,
      palf::PalfHandleGuard &palf_handle_guard,
      FetchRunTime &frt,
      obrpc::ObCdcLSFetchLogResp &resp);
  // Check the next LogEntry to determine whether need to continue fetching logs.
  void check_next_entry_(const LSN &next_lsn,
      const LogEntry &next_log_entry,
      obrpc::ObCdcLSFetchLogResp &resp,
      FetchRunTime &frt);
  // Fill Log Entry directly into resp_buf.
  // TODO Consider compression and decryption
  int prefill_resp_with_log_entry_(const ObLSID &ls_id,
      const LSN &lsn,
      LogEntry &log_entry,
      obrpc::ObCdcLSFetchLogResp &resp);

private:
  bool is_inited_;
  uint64_t           tenant_id_;
  ObLSService        *ls_service_;
};

// Some parameters and status during Fetch execution
struct FetchRunTime
{
  FetchRunTime();
  ~FetchRunTime();
  int init(const ObLogRpcIDType rpc_id,
      const int64_t rpc_start_tstamp,
      const int64_t upper_limit_ts = 0);

  inline bool is_stopped() const { return is_stopped_; }
  inline void stop(const char *stop_reason)
  {
    is_stopped_ = true;
    stop_reason_ = stop_reason;
  }

  inline bool is_ls_fall_behind() const
  {
    int64_t delay_time = rpc_start_tstamp_ - upper_limit_ts_;
    return delay_time > LS_FALL_BEHIND_THRESHOLD_TIME;
  }

  TO_STRING_KV(K(rpc_id_),
      K(rpc_start_tstamp_),
      K(upper_limit_ts_),
      K(rpc_deadline_),
      K(is_stopped_),
      K(stop_reason_),
      K(fetch_status_));

  // In params: config related
  // The unique identifier of a round of RPC, currently uses a timestamp to distinguish
  ObLogRpcIDType rpc_id_;
  int64_t rpc_start_tstamp_;
  int64_t upper_limit_ts_;
  int64_t rpc_deadline_;

  // Out params: control flow related
  bool is_stopped_;
  const char *stop_reason_;

  // the following are frequently changed
  //clog::ObReadCost read_cost_ CACHE_ALIGNED;
  //clog::ObIlogStorageQueryCost csr_cost_ CACHE_ALIGNED;
  obrpc::ObCdcFetchStatus fetch_status_ CACHE_ALIGNED;
};

} // namespace cdc
} // namespace oceanbase

#endif
