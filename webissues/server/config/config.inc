<?php
/*
* Configuration file for the WebIssues Server.
*/

/*
* Database configuration.
*
* Create the database and set correct values before running
* the setup.php script.
*/

$config[ 'db_engine' ]        = 'mysql'; // can be 'mysql' or 'pgsql'

$config[ 'db_host' ]          = 'localhost'; // can be 'host', 'host:port' or '/path/to/socket'
$config[ 'db_database' ]      = 'webissuesdb'; // name of the database
$config[ 'db_user' ]          = 'webissuesuser'; // database user login (if needed)
$config[ 'db_password' ]      = '111111'; // database user password (if needed)

$config[ 'db_prefix' ]        = 'we_'; // prefix of table names

/*
* Attachment storage configuration.
*
* Create the storage directory with write permissions for the HTTP server
* before running the setup.php script.
*
* The path can be absolute or relative to index.php. Set it to an empty string
* to disable storing attachments as files.
*
* Attachments smaller than min_size bytes will be stored in the database.
* Set min_size to 0 to store all attachments as files.
*/

$config[ 'storage_path' ]     = '/var/www/webissues-server/storage';
$config[ 'storage_min_size' ] = 4000;

/*
* Logging configuration.
*
* Enter a file path to enable a log.
*
* Paths can be absolute or relative to index.php.
*/

$config[ 'log_errors' ]       = '/tmp/errors.log';
$config[ 'log_commands' ]     = '/tmp/commands.log';
$config[ 'log_sql' ]          = '';
