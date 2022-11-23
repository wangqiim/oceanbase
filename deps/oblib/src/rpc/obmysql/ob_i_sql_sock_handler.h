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

#ifndef OCEANBASE_OBMYSQL_OB_I_SQL_SOCK_HANDLER_H_
#define OCEANBASE_OBMYSQL_OB_I_SQL_SOCK_HANDLER_H_
namespace oceanbase
{
namespace obmysql
{
class ObISqlSockHandler {
public:
  ObISqlSockHandler() {}
  virtual ~ObISqlSockHandler() {}
  virtual int on_readable(void* sess) = 0;
  virtual void on_close(void* sess, int err) = 0;
  virtual void on_flushed(void* sess) = 0;
  virtual int on_connect(void* sess, int fd) = 0;
};

}; // end namespace obmysql
}; // end namespace oceanbase

#endif /* OCEANBASE_OBMYSQL_OB_I_SQL_SOCK_HANDLER_H_ */

