#select /*+ READ_CONSISTENCY*/ * from `t1` where c1 = 'a' 'b' /*HINT+ READ_CONSISTENCY*/;
select * from t1 where c1 = 1;
select * from t1 where c1 = 111111111111111111111111111;
select * from t1 where c1 = 'a' /*READ_CONSISTENCY*/;
select d.t1.c1, sum(t1.c2) from d.t1 where d.t1.c1 > 0 and c2 + d.t1.c1 = 100 group by d.t1.c2 order by t.d1.c1 desc limit 0, 1;
select * from t1 where c1 = 'abc';
create table s002 (c1 int primary key, c2 varchar(50), unique index idx(c2(20)));
create table s001 (c1 int primary key, c2 varchar(50), unique key idx(c2(20)));
create table s001 (c1 int primary key, c2 varchar(50), unique idx(c2(20)));
use rongxuan;
start transaction read only;
start transaction read write;
start transaction with consistent snapshot, read only;
start transaction read write, with consistent snapshot;
alter user 'zdy' account lock;
alter user 'zdy' account unlock ;
select d.t1.c1, sum(t1.c2) from d.t1 where d.t1.c1 > 0 and c2 + d.t1.c1 = 100 group by d.t1.c2 order by t.d1.c1 desc limit 0, 1;
select c1, sum(d.t1.c2) from t1 where t1.c1 > 0 and c2 + t1.c1 = 100 group by t1.c2 order by t1.c1 desc limit 0, 1;
select t1.c1, sum(c2) from t1 where c1 > 0 and c2 + c1 = 100 group by c2 order by c1 desc limit 0, 1;
insert into t1 values(1, 2);
insert into t1.t1 values(1, 2);
insert into t1(c1) values(1), (2);
insert into t1(t1.c1) values(1), (2);
insert into d.t1(d.t1.c1) values(1), (2);
update t1 set d.t1.c2=t1.c1+1 where d.t1.c1 > 1 order by d.t1.c1 desc limit 0, 10;
update d.t1 set t1.c2=d.t1c1+1 where t1.c1 > 1 order by c1 desc limit 0, 10;
delete from d.t1 where d.t1.c2 > 10 order by c1 limit 0, 1;
select t1.c1, t2.c1 from d.t1 join d.t2 on d.t1.c1=t2.c1 where t1.c1>0;
select d.t1.c1, t2.c1 from d.t1 join t2 on t1.c1=t2.c1 where t1.c1>0;
select d.t1.c1, t2.c1 from d.t1 join t2 on c1=c1 where t1.c1>0;
insert into t1 value (1, 2), (3, 4) on duplicate key update d.t.c1 = t.c2 + 1, c2 = c2 + 3;
insert into d.t1 value (1, 2), (3, 4) on duplicate key update t.c1 = t.c2 + 1, d.t.c2 = t.c2 + 3;
create table rongxuan(c int primary key, c2 int);
create table rongxuan(rongxuan.c int primary key, c2 int);
create table rongxuan(d.rongxuan.c int primary key, c2 int);
drop table t1;
drop table oceanbase.t, t1;
alter table rongxuan add c3 int;
alter table rongxuan add rongxuan.c4 int;
alter table rongxuan add test.rongxuan.c5 int;
drop database rongxuan;
create database rongxuan;
create database if not exists rongxuan;
create database if not exists rongxuan default character set = 'utf8'  default collate = 'default_collate';
select * from d.t1 PARTITION(p1, p2);
delete from d.t1 PARTITION(p0, p1);
update d.t1 PARTITION (p2) SET id = 2 WHERE name = 'Jill';
INSERT INTO d.t1 PARTITION (p3, p4) VALUES (24, 'Tim', 'Greene', 3, 1),  (26, 'Linda', 'Mills', 2, 1);
REPLACE INTO d.t1 PARTITION (p0) VALUES (20, 'Jan', 'Jones', 3, 2);
SELECT e.id, s.city, d.name FROM e JOIN stores PARTITION (p1) ON e.id=s.id JOIN departments PARTITION (p0) ON e.id=d.id;
alter system switch replica leader zone = 'z1';
alter system switch replica leader server = '127.0.0.1:80';
alter system switch replica leader partition_id = '1%8@3001' server = '127.0.0.1:80';
alter system switch replica leader partition_id '1%8@3001' server '127.0.0.1:80';
alter system report replica;
alter system report replica server = '127.0.0.1:80';
alter system report replica zone = 'z1';
alter system recycle replica;
alter system recycle replica server = '127.0.0.1:80';
alter system recycle replica server '127.0.0.1:80';
alter system recycle replica zone = 'z1';
alter system recycle replica zone 'z1';
alter system major freeze;
alter system start merge zone = 'z1';
alter system suspend merge;
alter system suspend merge tenant = all;
alter system resume merge;
alter system resume merge tenant = all;
alter system clear roottable;
alter system clear roottable tenant = 'xxx';
select * from t1 where c1>ANY(select c1 from t2 where c2>1);
select * from t1 where c1>SOME(select c1 from t2 where c2>1);
select * from t1 where c1>ALL(select c1 from t2 where c2>1);
select * from t1 where c1>(select c1 from t2 where c2>1);
select * from t1 where c1<ANY(select c1 from t2 where c2>1);
select * from t1 where c1<SOME(select c1 from t2 where c2>1);
select * from t1 where c1<ALL(select c1 from t2 where c2>1);
select * from t1 where c1<(select c1 from t2 where c2>1);
select * from t1 where c1>=ANY(select c1 from t2 where c2>1);
select * from t1 where c1>=SOME(select c1 from t2 where c2>1);
select * from t1 where c1>=ALL(select c1 from t2 where c2>1);
select * from t1 where c1>=(select c1 from t2 where c2>1);
select * from t1 where c1<=ANY(select c1 from t2 where c2>1);
select * from t1 where c1<=SOME(select c1 from t2 where c2>1);
select * from t1 where c1<=ALL(select c1 from t2 where c2>1);
select * from t1 where c1<=(select c1 from t2 where c2>1);
select * from t1 where c1=ANY(select c1 from t2 where c2>1);
select * from t1 where c1=SOME(select c1 from t2 where c2>1);
select * from t1 where c1=ALL(select c1 from t2 where c2>1);
select * from t1 where c1=(select c1 from t2 where c2>1);
select * from t1 where c1!=ANY(select c1 from t2 where c2>1);
select * from t1 where c1!=SOME(select c1 from t2 where c2>1);
select * from t1 where c1!=ALL(select c1 from t2 where c2>1);
select * from t1 where c1!=(select c1 from t2 where c2>1);
select * from t1 where c1 in (select c1 from t2 where c2>1);
select * from t1 where c1 not in (select c1 from t2 where c2>1);
select * from t1 where exists (select c1 from t2 where c2>1);
select * from t1 where not exists (select c1 from t2 where c2>1);
select * from t1 where (select c1 from t1) like (select c2 from t2);
select * from t1 where (select c1 from t1) not like (select c2 from t2);
select * from t1 where (c1) in (select c1 from t2);
select * from t1 where (c1, c2) in (select c1, c2 from t2);
select * from t1 where ROW(c1, c2) in (select c1, c2 from t2);
set names latin1;
set names 'latin1';
set names utf8 collate 'utf8_general_ci';
set names utf8 collate utf8_general_ci;
set character set utf8;
set character set 'utf8';
set charset utf8;
select _utf8 'abc', _utf8mb4 'def' collate utf8mb4_general_ci from t1 where c1 collate utf8_bin = 'xyz' collate utf8_bin;
select * from t1 where c1=?;
select * from t1 where c1>?;
select * from t1 where (select c1 from t1 where c1 = ?) not like (select c2 from t2 where c2=?);
#test join syntax with no  join_condition;
select * from t1 join t2;
select * from t1 inner join t2;
select * from t1 cross join t2;
select * from t1 cross join t2 join t3;
select "1234";
select '1234';
create table test(c1 varchar(3) binary);
create table test(c1 varchar(3) binary charset utf8mb4);
replace into test values(1,2);
insert ignore into test values(1,2);
insert ignore into test values(1,2) on duplicate key update c2 = c1 + 1;
update ignore tt set c1 = 1;
create table t1(c1 int primary key) partition by range(c1) partition size auto partition auto;
create table t1(c1 int) partition by range() partition size 10000 partition auto;
