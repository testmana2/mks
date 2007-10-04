-- Database Schema for MySQL 4.0

-- 
-- Table structure for attr_types
-- 

CREATE TABLE {attr_types} (
  attr_id int(11) NOT NULL auto_increment,
  type_id int(11) NOT NULL default '0',
  attr_name varchar(40) NOT NULL default '',
  attr_def longtext NOT NULL,
  PRIMARY KEY (attr_id),
  KEY type_id (type_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for attr_values
-- 

CREATE TABLE {attr_values} (
  issue_id int(11) NOT NULL default '0',
  attr_id int(11) NOT NULL default '0',
  attr_value varchar(80) NOT NULL default '',
  PRIMARY KEY (issue_id,attr_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for changes
-- 

CREATE TABLE {changes} (
  change_id int(11) NOT NULL default '0',
  issue_id int(11) NOT NULL default '0',
  attr_id int(11) NOT NULL default '0',
  value_old varchar(80) NOT NULL default '',
  value_new varchar(80) NOT NULL default '',
  PRIMARY KEY (change_id),
  KEY issue_id (issue_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for comments
-- 

CREATE TABLE {comments} (
  comment_id int(11) NOT NULL default '0',
  issue_id int(11) NOT NULL default '0',
  comment_text longtext NOT NULL,
  PRIMARY KEY (comment_id),
  KEY issue_id (issue_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for files
-- 

CREATE TABLE {files} (
  file_id int(11) NOT NULL default '0',
  issue_id int(11) NOT NULL default '0',
  file_name varchar(40) NOT NULL default '',
  file_size int(11) NOT NULL default '0',
  file_data longblob,
  file_descr varchar(80) NOT NULL default '',
  file_storage tinyint(4) NOT NULL default '0',
  PRIMARY KEY (file_id),
  KEY issue_id (issue_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for folders
-- 

CREATE TABLE {folders} (
  folder_id int(11) NOT NULL auto_increment,
  project_id int(11) NOT NULL default '0',
  type_id int(11) NOT NULL default '0',
  folder_name varchar(40) NOT NULL default '',
  stamp_id int(11) NOT NULL default '0',
  PRIMARY KEY (folder_id),
  KEY project_id (project_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for issue_types
-- 

CREATE TABLE {issue_types} (
  type_id int(11) NOT NULL auto_increment,
  type_name varchar(40) NOT NULL default '',
  PRIMARY KEY (type_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for issues
-- 

CREATE TABLE {issues} (
  issue_id int(11) NOT NULL default '0',
  folder_id int(11) NOT NULL default '0',
  issue_name varchar(80) NOT NULL default '',
  stamp_id int(11) NOT NULL default '0',
  PRIMARY KEY (issue_id),
  KEY folder_id (folder_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for projects
-- 

CREATE TABLE {projects} (
  project_id int(11) NOT NULL auto_increment,
  project_name varchar(40) NOT NULL default '',
  PRIMARY KEY (project_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for rights
-- 

CREATE TABLE {rights} (
  project_id int(11) NOT NULL default '0',
  user_id int(11) NOT NULL default '0',
  project_access tinyint(4) NOT NULL default '0',
  UNIQUE KEY project_id (project_id,user_id),
  KEY user_id (user_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for server
-- 

CREATE TABLE {server} (
  server_name varchar(40) NOT NULL default '',
  server_uuid char(36) NOT NULL default '',
  db_version varchar(10) NOT NULL default ''
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for stamps
-- 

CREATE TABLE {stamps} (
  stamp_id int(11) NOT NULL auto_increment,
  user_id int(11) NOT NULL default '0',
  stamp_time int(11) NOT NULL default '0',
  PRIMARY KEY (stamp_id)
) DEFAULT CHARACTER SET utf8;

-- 
-- Table structure for users
-- 

CREATE TABLE {users} (
  user_id int(11) NOT NULL auto_increment,
  user_login varchar(40) NOT NULL default '',
  user_name varchar(40) NOT NULL default '',
  user_passwd char(32) NOT NULL default '',
  user_access tinyint(4) NOT NULL default '0',
  PRIMARY KEY (user_id),
  UNIQUE KEY user_login (user_login)
) DEFAULT CHARACTER SET utf8;
