-- Database Schema for PostgreSQL

-- 
-- Table structure for attr_types
-- 

CREATE TABLE {attr_types} (
  attr_id serial,
  type_id integer NOT NULL default '0',
  attr_name varchar(40) NOT NULL default '',
  attr_def text NOT NULL,
  PRIMARY KEY (attr_id)
);
CREATE INDEX {attr_types}_type_idx ON {attr_types}(type_id);

-- 
-- Table structure for attr_values
-- 

CREATE TABLE {attr_values} (
  issue_id integer NOT NULL default '0',
  attr_id integer NOT NULL default '0',
  attr_value varchar(80) NOT NULL default '',
  PRIMARY KEY (issue_id, attr_id)
);

-- 
-- Table structure for changes
-- 

CREATE TABLE {changes} (
  change_id integer NOT NULL default '0',
  issue_id integer NOT NULL default '0',
  attr_id integer NOT NULL default '0',
  value_old varchar(80) NOT NULL default '',
  value_new varchar(80) NOT NULL default '',
  PRIMARY KEY (change_id)
);
CREATE INDEX {changes}_issue_idx ON {changes}(issue_id);

-- 
-- Table structure for comments
-- 

CREATE TABLE {comments} (
  comment_id integer NOT NULL default '0',
  issue_id integer NOT NULL default '0',
  comment_text text NOT NULL,
  PRIMARY KEY (comment_id)
);
CREATE INDEX {comments}_issue_idx ON {comments}(issue_id);

-- 
-- Table structure for files
-- 

CREATE TABLE {files} (
  file_id integer NOT NULL default '0',
  issue_id integer NOT NULL default '0',
  file_name varchar(40) NOT NULL default '',
  file_size integer NOT NULL default '0',
  file_data bytea,
  file_descr varchar(80) NOT NULL default '',
  file_storage smallint NOT NULL default '0',
  PRIMARY KEY (file_id)
);
CREATE INDEX {files}_issue_idx ON {files}(issue_id);

-- 
-- Table structure for folders
-- 

CREATE TABLE {folders} (
  folder_id serial,
  project_id integer NOT NULL default '0',
  type_id integer NOT NULL default '0',
  folder_name varchar(40) NOT NULL default '',
  stamp_id integer NOT NULL default '0',
  PRIMARY KEY (folder_id)
);
CREATE INDEX {folders}_project_idx ON {folders}(project_id);

-- 
-- Table structure for issue_types
-- 

CREATE TABLE {issue_types} (
  type_id serial,
  type_name varchar(40) NOT NULL default '',
  PRIMARY KEY (type_id)
);

-- 
-- Table structure for issues
-- 

CREATE TABLE {issues} (
  issue_id integer NOT NULL default '0',
  folder_id integer NOT NULL default '0',
  issue_name varchar(80) NOT NULL default '',
  stamp_id integer NOT NULL default '0',
  PRIMARY KEY (issue_id)
);
CREATE INDEX {issues}_folder_idx ON {issues}(folder_id);

-- 
-- Table structure for projects
-- 

CREATE TABLE {projects} (
  project_id serial,
  project_name varchar(40) NOT NULL default '',
  PRIMARY KEY (project_id)
);

-- 
-- Table structure for rights
-- 

CREATE TABLE {rights} (
  project_id integer NOT NULL default '0',
  user_id integer NOT NULL default '0',
  project_access smallint NOT NULL default '0',
  PRIMARY KEY (project_id, user_id)
);
CREATE INDEX {rights}_user_idx ON {rights}(user_id);

-- 
-- Table structure for server
-- 

CREATE TABLE {server} (
  server_name varchar(40) NOT NULL default '',
  server_uuid char(36) NOT NULL default '',
  db_version varchar(10) NOT NULL default ''
);

-- 
-- Table structure for stamps
-- 

CREATE TABLE {stamps} (
  stamp_id serial,
  user_id integer NOT NULL default '0',
  stamp_time integer NOT NULL default '0',
  PRIMARY KEY (stamp_id)
);

-- 
-- Table structure for users
-- 

CREATE TABLE {users} (
  user_id serial,
  user_login varchar(40) NOT NULL default '',
  user_name varchar(40) NOT NULL default '',
  user_passwd char(32) NOT NULL default '',
  user_access smallint NOT NULL default '0',
  PRIMARY KEY (user_id),
  UNIQUE (user_login)
);
