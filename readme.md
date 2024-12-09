# MySQL Connection Pool

* 数据库创建
  * `create database test;`
  * `grant all ON test.* to 'pulsar'@'localhost';`
  * `use test;`
  * `create table user(id int(11) PRIMARY KEY NOT NULL AUTO_INCREMENT, name varchar(50), age int(11), sex enum('male', 'female'));`