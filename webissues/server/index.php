<?php
/**************************************************************************
* This file is part of the WebIssues Server program
* Copyright (C) 2006-2007 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

require_once( 'config/config.inc' );

require_once( 'include/database.inc' );
require_once( 'include/protocol.inc' );
require_once( 'include/validation.inc' );

define( 'ACCESS_NONE', 0 );
define( 'ACCESS_NORMAL', 1 );
define( 'ACCESS_ADMIN', 2 );

main();

function main()
{
    global $config;

    wi_send_headers();
	wi_start_reply ();

    $args = wi_get_command();
    $keyword = $args[ 0 ];
    wi_open_database();
    
    if ( $keyword == 'HELLO' ) {
        $query = 'SELECT server_name, server_uuid FROM {server}';
        $row = wi_query_row( $query );
        if ( !$row )
            wi_error( 500 );

        wi_reply( $row, 'SERVER'  );
        wi_finish_reply ();
    }

    if ( $keyword == 'LOGIN' ) {
        $login = wi_string( $args[ 1 ], 40 );
        $password = wi_string( $args[ 2 ], 40 );

        unset( $_SESSION[ 'user_id' ] );
        unset( $_SESSION[ 'user_access' ] );

        $query = 'SELECT user_id, user_name, user_access FROM {users}'
            . ' WHERE user_login = BINARY %s AND user_passwd = MD5(%s)';

        $row = wi_query_row( $query, $login, $password );
        if ( $row ) {
            if ( $row[ 'user_access' ] == ACCESS_NONE )
                wi_error( 302 );

            $_SESSION[ 'user_id' ] = (int)$row[ 'user_id' ];
            $_SESSION[ 'user_access' ] = (int)$row[ 'user_access' ];

            wi_reply( $row ,'LOGINED_USER');
            wi_finish_reply ();
        }

        wi_error( 302 );
    }

    if ( !isset( $_SESSION[ 'user_id' ] ) )
        wi_error( 300 );

    if ( $keyword == 'LIST USERS' ) {
        $query = 'SELECT user_id, user_login, user_name, user_access FROM {users}';
        $rs = wi_query( $query );

        $reply = array();
        while ( $row = wi_fetch( $rs ) ) {
            wi_reply ( $row, 'USER');
        }

        $query = 'SELECT r.project_id, r.user_id, r.project_access FROM {rights} AS r';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            $query .= ' JOIN {rights} AS r2 ON r2.project_id = r.project_id AND r2.user_id = %d';
        $rs = wi_query( $query, $_SESSION[ 'user_id' ] );

        while ( $row = wi_fetch( $rs ) ) {
            $user_id = $row[ 'user_id' ];
            $project_id = $row[ 'project_id' ];
            $access = $row[ 'project_access' ];
            wi_reply ( array('user_id'=>$user_id, 'project_id'=> $project_id, 'access'=> $access), 'USER_ACCESS');
        }

		wi_finish_reply ();
    }

    if ( $keyword == 'ADD USER' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $login = wi_string( $args[ 1 ], 40 );
        $name = wi_string( $args[ 2 ], 40 );

        $query = 'SELECT user_id FROM {users} WHERE user_login = BINARY %s OR user_name = BINARY %s';
        if ( wi_query_row( $query, $login, $name ) )
            wi_error( 324 );

        $password = wi_string( $args[ 3 ], 40 );

        $query = 'INSERT INTO {users} ( user_login, user_name, user_passwd, user_access ) VALUES ( %s, %s, MD5( %s ), %d )';
        wi_query( $query, $login, $name, $password, ACCESS_NORMAL );

        $user_id = wi_insert_id( 'users', 'user_id' );
        wi_reply( array ('user_id'=> $user_id));
        wi_finish_reply ();	
    }

    if ( $keyword == 'SET PASSWORD' ) {
        $user_id = $args[ 1 ];

        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN && $user_id != $_SESSION[ 'user_id' ] )
            wi_error( 301 );

        $query = 'SELECT user_id FROM {users} WHERE user_id = %d';
        if ( !wi_query_row( $query, $user_id ) )
            wi_error( 314 );

        $password = wi_string( $args[ 2 ], 40 );

        $query = 'UPDATE {users} SET user_passwd = MD5( %s ) WHERE user_id = %d';
        wi_query( $query, $password, $user_id );

        wi_reply( array('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'RENAME USER' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $user_id = $args[ 1 ];

        $query = 'SELECT user_name FROM {users} WHERE user_id = %d';
        if ( $row = wi_query_row( $query, $user_id ) )
            $old_name = $row[ 'user_name' ];
        else
            wi_error( 314 );

        $new_name = wi_string( $args[ 2 ], 40 );

        if ( $new_name == $old_name )
        {
			wi_reply( array ('result'=>'NULL') );
			wi_finish_reply ();
		}

        $query = 'SELECT user_id FROM {users} WHERE user_name = BINARY %s';
        if ( wi_query_row( $query, $new_name ) )
            wi_error( 324 );

        $query = 'UPDATE {users} SET user_name = %s WHERE user_id = %d';
        wi_query( $query, $new_name, $user_id );

        wi_reply( array('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'GRANT USER' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $user_id = $args[ 1 ];

        $query = 'SELECT user_access FROM {users} WHERE user_id = %d';
        if ( $row = wi_query_row( $query, $user_id ) )
            $old_access = $row[ 'user_access' ];
        else
            wi_error( 314 );

        if ( $user_id == 1 )
            wi_error( 301 );

        $new_access = $args[ 2 ];

        if ( $new_access < ACCESS_NONE || $new_access > ACCESS_ADMIN )
            wi_error( 341 );

        if ( $new_access == $old_access )
        {
            wi_reply( array ('result'=>'NULL') );
            wi_finish_reply ();
		}

        $query = 'UPDATE {users} SET user_access = %d WHERE user_id = %d';
        wi_query( $query, $new_access, $user_id );

        wi_reply( array('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'GRANT MEMBER' ) {
        $user_id = $args[ 1 ];

        $query = 'SELECT user_id FROM {users} WHERE user_id = %d';
        if ( !wi_query_row( $query, $user_id ) )
            wi_error( 314 );

        $project_id = $args[ 2 ];

        if ( $_SESSION[ 'user_access' ] == ACCESS_ADMIN ) {
            $query = 'SELECT %3$d AS project_access FROM {projects} WHERE project_id = %1$d';
        } else {
            $query = 'SELECT r.project_access FROM {projects} AS p'
                . ' JOIN {rights} AS r ON r.project_id = p.project_id AND r.user_id = %2$d'
                . ' WHERE p.project_id = %1$d';
        }

        if ( $row = wi_query_row( $query, $project_id, $_SESSION[ 'user_id' ], ACCESS_ADMIN ) ) {
            if ( $row[ 'project_access' ] < ACCESS_ADMIN )
                wi_error( 301 );
        } else {
            wi_error( 310 );
        }

        $new_access = $args[ 3 ];

        if ( $new_access < ACCESS_NONE || $new_access > ACCESS_ADMIN )
            wi_error( 341 );

        $query = 'SELECT project_access FROM {rights} WHERE project_id = %d AND user_id = %d';
        if ( $row = wi_query_row( $query, $project_id, $user_id ) )
            $old_access = $row[ 'project_access' ];
        else
            $old_access = ACCESS_NONE;

        if ( $new_access == $old_access )
        {
			wi_reply( array ('result'=>'NULL') );
			wi_finish_reply ();
		}

        if ( $old_access == ACCESS_NONE )
            $query = 'INSERT INTO {rights} ( project_id, user_id, project_access ) VALUES ( %1$d, %2$d, %3$d )';
        else if ( $new_access == ACCESS_NONE )
            $query = 'DELETE FROM {rights} WHERE project_id = %1$d AND user_id = %2$d';
        else
            $query = 'UPDATE {rights} SET project_access = %3$d WHERE project_id = %1$d AND user_id = %2$d';
        wi_query( $query, $project_id, $user_id, $new_access );

        wi_reply( array('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'LIST TYPES' ) {
        $query = 'SELECT type_id, type_name FROM {issue_types}';
        $rs = wi_query( $query );

        $reply = array();
        while ( $row = wi_fetch( $rs ) ) {
            wi_reply ( $row,'ISSUE_TYPE');
        }

        $query = 'SELECT attr_id, type_id, attr_name, attr_def FROM {attr_types}';
        $rs = wi_query( $query );

        while ( $row = wi_fetch( $rs ) ) {
            wi_reply ( $row, 'ATTRIBUTE_TYPE' ); //do it right name?
        }
        wi_finish_reply ();
    }

    if ( $keyword == 'ADD TYPE' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $name = wi_string( $args[ 1 ], 40 );

        $query = 'SELECT type_id FROM {issue_types} WHERE type_name = BINARY %s';
        if ( wi_query_row( $query, $name ) )
            wi_error( 325 );

        $query = 'INSERT INTO {issue_types} ( type_name ) VALUES ( %s )';
        wi_query( $query, $name );
        $type_id = wi_insert_id( 'issue_types', 'type_id' );
        wi_reply( array ( 'type_id' =>$type_id ));
        wi_finish_reply ();
    }

    if ( $keyword == 'RENAME TYPE' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $type_id = $args[ 1 ];

        $query = 'SELECT type_name FROM {issue_types} WHERE type_id = %d';
        if ( $row = wi_query_row( $query, $type_id ) )
            $old_name = $row[ 'type_name' ];
        else
            wi_error( 315 );

        $new_name = wi_string( $args[ 2 ], 40 );

        if ( $new_name == $old_name )
        {
			wi_reply( array('result'=>'NULL') );
        	wi_finish_reply ();
		}

        $query = 'SELECT type_id FROM {issue_types} WHERE type_name = BINARY %s';
        if ( wi_query_row( $query, $new_name ) )
            wi_error( 325 );

        $query = 'UPDATE {issue_types} SET type_name = %s WHERE type_id = %d';
        wi_query( $query, $new_name, $type_id );

        wi_reply( array('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'DELETE TYPE' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $type_id = $args[ 1 ];

        $query = 'SELECT type_id FROM {issue_types} WHERE type_id = %d';
        if ( !wi_query_row( $query, $type_id ) )
            wi_error( 315 );

        $query = 'SELECT folder_id FROM {folders} WHERE type_id = %d';
        if ( wi_query_row( $query, $type_id ) )
            wi_error( 335 );

        $query = 'DELETE FROM {attr_types} WHERE type_id = %d';
        wi_query( $query, $type_id );

        $query = 'DELETE FROM {issue_types} WHERE type_id = %d';
        wi_query( $query, $type_id );

        wi_reply( array ('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'ADD ATTRIBUTE' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $type_id = $args[ 1 ];

        $query = 'SELECT type_name FROM {issue_types} WHERE type_id = %d';
        if ( !wi_query_row( $query, $type_id ) )
            wi_error( 315 );

        $name = wi_string( $args[ 2 ], 40 );

        $query = 'SELECT attr_id FROM {attr_types} WHERE type_id = %d AND attr_name = BINARY %s';
        if ( wi_query_row( $query, $type_id, $name ) )
            wi_error( 326 );

        $def = $args[ 3 ];

        wi_verify_definition( $def );

        $query = 'INSERT INTO {attr_types} ( type_id, attr_name, attr_def ) VALUES ( %d, %s, %s )';
        wi_query( $query, $type_id, $name, $def );

        $attr_id = wi_insert_id( 'attr_types', 'attr_id' );
        wi_reply( array ('attribute_id'=>$attr_id) );
        wi_finish_reply ();
    }

    if ( $keyword == 'RENAME ATTRIBUTE' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $attr_id = $args[ 1 ];

        $query = 'SELECT type_id, attr_name FROM {attr_types} WHERE attr_id = %d';
        if ( $row = wi_query_row( $query, $attr_id ) ) {
            $type_id = $row[ 'type_id' ];
            $old_name = $row[ 'attr_name' ];
        } else {
            wi_error( 316 );
        }

        $new_name = wi_string( $args[ 2 ], 40 );

        if ( $new_name == $old_name )
        {
            wi_reply( array ('result'=>'NULL') );
            wi_finish_reply ();
		}

        $query = 'SELECT attr_id FROM {attr_types} WHERE type_id = %d AND attr_name = BINARY %s';
        if ( wi_query_row( $query, $type_id, $new_name ) )
            wi_error( 326 );

        $query = 'UPDATE {attr_types} SET attr_name = %s WHERE attr_id = %d';
        wi_query( $query, $new_name, $attr_id );

        wi_reply( array ('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'MODIFY ATTRIBUTE' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $attr_id = $args[ 1 ];

        $query = 'SELECT attr_def FROM {attr_types} WHERE attr_id = %d';
        if ( $row = wi_query_row( $query, $attr_id ) )
            $old_def = $row[ 'attr_def' ];
        else
            wi_error( 316 );

        $new_def = $args[ 2 ];

        wi_verify_definition( $new_def );

        if ( $new_def == $old_def )
		{
			wi_reply( array ('result'=>'NULL') );
            wi_finish_reply ();
		}
        $query = 'UPDATE {attr_types} SET attr_def = %s WHERE attr_id = %d';
        wi_query( $query, $new_def, $attr_id );

        wi_reply( array('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'DELETE ATTRIBUTE' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $attr_id = $args[ 1 ];

        $query = 'SELECT attr_id FROM {attr_types} WHERE attr_id = %d';
        if ( !wi_query_row( $query, $attr_id ) )
            wi_error( 316 );

        $query = 'DELETE FROM {attr_values} WHERE attr_id = %d';
        wi_query( $query, $attr_id );

        $query = 'DELETE FROM {changes} WHERE attr_id = %d';
        wi_query( $query, $attr_id );

        $query = 'DELETE FROM {attr_types} WHERE attr_id = %d';
        wi_query( $query, $attr_id );

        wi_reply( array('result'=>'OK') );
    }

    if ( $keyword == 'LIST PROJECTS' ) {

        $user_id = $_SESSION[ 'user_id' ];

        $query = 'SELECT p.project_id, p.project_name FROM {projects} AS p';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            $query .= ' JOIN {rights} AS r ON r.project_id = p.project_id AND r.user_id = %d';
        $rs = wi_query( $query, $user_id );

        $reply = array();
        /*while ( */$row = wi_fetch( $rs ); /*) {*/
            $project_id = $row[ 'project_id' ];
            $name = wi_quote( $row[ 'project_name' ] );
            wi_reply ( $row,'PROJECT');
//         }

//         $query = 'SELECT f.folder_id, f.project_id, f.folder_name, f.type_id, f.stamp_id FROM {folders} AS f';
//         if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
//             $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %d';
//         $rs = wi_query( $query, $user_id );
// 
//         while ( $row = wi_fetch( $rs ) ) {
//             $folder_id = $row[ 'folder_id' ];
//             $project_id = $row[ 'project_id' ];
//             $name = wi_quote( $row[ 'folder_name' ] );
//             $type_id = $row[ 'type_id' ];
//             $stamp = $row[ 'stamp_id' ];
//             wi_reply ( $row, 'FOLDER');
//         }
		wi_finish_reply ();
    }

    if ( $keyword == 'ADD PROJECT' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $name = wi_string( $args[ 1 ], 40 );

        $query = 'SELECT project_id FROM {projects} WHERE project_name = BINARY %s';
        if ( wi_query_row( $query, $name ) )
            wi_error( 320 );

        $query = 'INSERT INTO {projects} ( project_name ) VALUES ( %s )';
        wi_query( $query, $name );

        $project_id = wi_insert_id( 'projects', 'project_id' );
        wi_reply( array ('project_id' =>$project_id));
        wi_finish_reply ();
    }

    if ( $keyword == 'RENAME PROJECT' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $project_id = $args[ 1 ];

        $query = 'SELECT project_name FROM {projects} WHERE project_id = %d';
        if ( $row = wi_query_row( $query, $project_id ) )
            $old_name = $row[ 'project_name' ];
        else
            wi_error( 310 );

        $new_name = wi_string( $args[ 2 ], 40 );

        if ( $new_name == $old_name )
		{
			wi_reply( array ('result'=>'NULL') );
			wi_finish_reply ();
		}

        $query = 'SELECT project_id FROM {projects} WHERE project_name = BINARY %s';
        if ( wi_query_row( $query, $new_name ) )
            wi_error( 320 );

        $query = 'UPDATE {projects} SET project_name = %s WHERE project_id = %d';
        wi_query( $query, $new_name, $project_id );

        wi_reply( array ('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'DELETE PROJECT' ) {
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            wi_error( 301 );

        $project_id = $args[ 1 ];

        $query = 'SELECT project_id FROM {projects} WHERE project_id = %d';
        if ( !wi_query_row( $query, $project_id ) )
            wi_error( 315 );

        $query = 'SELECT folder_id FROM {folders} WHERE project_id = %d';
        if ( wi_query_row( $query, $project_id ) )
            wi_error( 330 );

        $query = 'DELETE FROM {rights} WHERE project_id = %d';
        wi_query( $query, $project_id );

        $query = 'DELETE FROM {projects} WHERE project_id = %d';
        wi_query( $query, $project_id );

        wi_reply( array('result'=>'OK') );
    }

    if ( $keyword == 'ADD FOLDER' ) {
        $project_id = $args[ 1 ];

        if ( $_SESSION[ 'user_access' ] == ACCESS_ADMIN ) {
            $query = 'SELECT %3$d AS project_access FROM {projects} WHERE project_id = %1$d';
        } else {
            $query = 'SELECT r.project_access FROM {projects} AS p'
                . ' JOIN {rights} AS r ON r.project_id = p.project_id AND r.user_id = %2$d'
                . ' WHERE p.project_id = %1$d';
        }

        if ( $row = wi_query_row( $query, $project_id, $_SESSION[ 'user_id' ], ACCESS_ADMIN ) ) {
            if ( $row[ 'project_access' ] < ACCESS_ADMIN )
                wi_error( 301 );
        } else {
            wi_error( 310 );
        }

        $type_id = $args[ 2 ];

        $query = 'SELECT type_id FROM {issue_types} WHERE type_id = %d';
        if ( !wi_query_row( $query, $type_id ) )
            wi_error( 315 );

        $name = wi_string( $args[ 3 ], 40 );

        $query = 'SELECT folder_id FROM {folders} WHERE project_id = %d AND folder_name = BINARY %s';
        if ( wi_query_row( $query, $project_id, $name ) )
            wi_error( 321 );

        $query = 'INSERT INTO {folders} ( project_id, type_id, folder_name, stamp_id ) VALUES ( %d, %d, %s, 0 )';
        wi_query( $query, $project_id, $type_id, $name );

        $folder_id = wi_insert_id( 'folders', 'folder_id' );
        wi_reply( array('folder_id'=>$folder_id) );
        wi_finish_reply ();
    }

    if ( $keyword == 'RENAME FOLDER' ) {
        $folder_id = $args[ 1 ];

        if ( $_SESSION[ 'user_access' ] == ACCESS_ADMIN ) {
            $query = 'SELECT project_id, folder_name, %3$d AS project_access'
            . ' FROM {folders} WHERE folder_id = %1$d';
        } else {
            $query = 'SELECT f.project_id, f.folder_name, r.project_access FROM {folders} AS f'
                . ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d'
                . ' WHERE f.folder_id = %1$d';
        }

        if ( $row = wi_query_row( $query, $folder_id, $_SESSION[ 'user_id' ], ACCESS_ADMIN ) ) {
            if ( $row[ 'project_access' ] < ACCESS_ADMIN )
                wi_error( 301 );
            $project_id = $row[ 'project_id' ];
            $old_name = $row[ 'folder_name' ];
        } else {
            wi_error( 311 );
        }

        $new_name = wi_string( $args[ 2 ], 40 );

        if ( $new_name == $old_name )
		{
			wi_reply( array ('result'=>'NULL') );
			wi_finish_reply ();
		}

        $query = 'SELECT folder_id FROM {folders} WHERE project_id = %d AND folder_name = BINARY %s';
        if ( wi_query_row( $query, $project_id, $new_name ) )
            wi_error( 321 );

        $query = 'UPDATE {folders} SET folder_name = %s WHERE folder_id = %d';
        wi_query( $query, $new_name, $folder_id );

        wi_reply( array('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'DELETE FOLDER' ) {
        $folder_id = $args[ 1 ];

        if ( $_SESSION[ 'user_access' ] == ACCESS_ADMIN ) {
            $query = 'SELECT %3$d AS project_access FROM {folders} WHERE folder_id = %1$d';
        } else {
            $query = 'SELECT r.project_access FROM {folders} AS f'
                . ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d'
                . ' WHERE f.folder_id = %1$d';
        }

        if ( $row = wi_query_row( $query, $folder_id, $_SESSION[ 'user_id' ], ACCESS_ADMIN ) ) {
            if ( $row[ 'project_access' ] < ACCESS_ADMIN )
                wi_error( 301 );
        } else {
            wi_error( 311 );
        }

        $query = 'SELECT issue_id FROM {issues} WHERE folder_id = %d';
        if ( wi_query_row( $query, $folder_id ) )
            wi_error( 331 );

        $query = 'DELETE FROM {folders} WHERE folder_id = %d';
        wi_query( $query, $folder_id );

        wi_reply( array('result'=>'OK') );
        wi_finish_reply ();
    }

    if ( $keyword == 'LIST ISSUES' ) {
        $folder_id = $args[ 1 ];

        $query = 'SELECT f.stamp_id FROM {folders} AS f';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        $query .= ' WHERE f.folder_id = %1$d';

        if ( $row = wi_query_row( $query, $folder_id, $_SESSION[ 'user_id' ] ) )
            $last_stamp = $row[ 'stamp_id' ];
        else
            wi_error( 311 );

        $from_stamp = $args[ 2 ];

        if ( $from_stamp >= $last_stamp )
        {
			wi_reply( array ('result'=>'NULL') );
			wi_finish_reply ();
		}

        $query = 'SELECT i.issue_id, i.issue_name, i.stamp_id,'
            . ' sc.stamp_time AS created, sc.user_id AS created_by,'
            . ' sm.stamp_time AS modified, sm.user_id AS modified_by'
            . ' FROM {issues} AS i'
            . ' JOIN {stamps} AS sc ON sc.stamp_id = i.issue_id'
            . ' JOIN {stamps} AS sm ON sm.stamp_id = i.stamp_id'
            . ' WHERE i.folder_id = %d AND i.stamp_id > %d';
        $rs = wi_query( $query, $folder_id, $from_stamp );

        while ( $row = wi_fetch( $rs ) ) {
            wi_reply ( $row, 'ISSUE');
        }

        $query = 'SELECT a.issue_id, a.attr_id, a.attr_value FROM {attr_values} AS a'
            . ' JOIN {issues} AS i ON i.issue_id = a.issue_id AND i.folder_id = %d AND i.stamp_id > %d';
        $rs = wi_query( $query, $folder_id, $from_stamp );

        while ( $row = wi_fetch( $rs ) ) {
            wi_reply ( $row, 'ISSUE_VALUE');
        }

        wi_finish_reply ();
    }

    if ( $keyword == 'GET DETAILS' ) {
        $issue_id = $args[ 1 ];

        $query = 'SELECT i.folder_id, i.issue_name, i.stamp_id,'
            . ' sc.stamp_time AS created, sc.user_id AS created_by,'
            . ' sm.stamp_time AS modified, sm.user_id AS modified_by'
            . ' FROM {issues} AS i'
            . ' JOIN {stamps} AS sc ON sc.stamp_id = i.issue_id'
            . ' JOIN {stamps} AS sm ON sm.stamp_id = i.stamp_id';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN ) {
            $query .= ' JOIN {folders} AS f ON f.folder_id = i.folder_id';
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        }
        $query .= ' WHERE i.issue_id = %1$d';

        $reply = array();
        if ( $row = wi_query_row( $query, $issue_id, $_SESSION[ 'user_id' ] ) ) {
            wi_reply ( $row, 'ISSUE');
	        } else {
    	        wi_error( 312 );
        	}

        $from_stamp = $args[ 2 ];

         if ( $from_stamp >= $stamp )
         {
 			wi_reply( array ('result'=>'NULL') );
 			wi_finish_reply ();
 		}

        $query = 'SELECT attr_id, attr_value FROM {attr_values} WHERE issue_id = %d';
        $rs = wi_query( $query, $issue_id );

        while ( $row = wi_fetch( $rs ) ) {
            wi_reply ($row, 'ISSUE_VALUE');
        }

        $query = 'SELECT c.comment_id, s.stamp_time, s.user_id, c.comment_text'
            . ' FROM {comments} AS c'
            . ' JOIN {stamps} AS s ON s.stamp_id = c.comment_id'
            . ' WHERE c.issue_id = %d AND s.stamp_id > %d';
        $rs = wi_query( $query, $issue_id, $from_stamp );

        while ( $row = wi_fetch( $rs ) ) {
            wi_reply ( $row, 'ISSUE_COMMENT');
        }

        $query = 'SELECT f.file_id, s.stamp_time, s.user_id, f.file_name, f.file_size, f.file_descr'
            . ' FROM {files} AS f'
            . ' JOIN {stamps} AS s ON s.stamp_id = f.file_id'
            . ' WHERE f.issue_id = %d AND s.stamp_id > %d';
        $rs = wi_query( $query, $issue_id, $from_stamp );

        while ( $row = wi_fetch( $rs ) ) {
            wi_reply ( $row, 'ISSUE_ATACHMENT');
        }

        $query = 'SELECT c.change_id, s.stamp_time, s.user_id, c.attr_id, c.value_old, c.value_new'
            . ' FROM {changes} AS c'
            . ' JOIN {stamps} AS s ON s.stamp_id = c.change_id'
            . ' WHERE c.issue_id = %d AND s.stamp_id > %d';
        $rs = wi_query( $query, $issue_id, $from_stamp );

        while ( $row = wi_fetch( $rs ) ) {
            wi_reply ($row, 'ISSUE_HISTORY');
        }

        wi_finish_reply ();
    }

    if ( $keyword == 'ADD ISSUE' ) {
        $folder_id = $args[ 1 ];

        $query = 'SELECT f.type_id FROM {folders} AS f';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        $query .= ' WHERE f.folder_id = %1$d';

        if ( $row = wi_query_row( $query, $folder_id, $_SESSION[ 'user_id' ] ) )
            $type_id = $row[ 'type_id' ];
        else
            wi_error( 311 );

        $query = 'SELECT attr_id, attr_def FROM {attr_types} WHERE type_id = %d';
        $rs = wi_query( $query, $type_id );

        $attrs = array();
        while ( $row = wi_fetch( $rs ) ) {
            $attr_id = $row[ 'attr_id' ];
            $value = wi_get_default_value( $row[ 'attr_def' ] );
            if ( $value != '' )
                $attrs[ $attr_id ] = $value;
        }

        $name = wi_string( $args[ 2 ], 80 );

        $query = 'INSERT INTO {stamps} ( user_id, stamp_time ) VALUES ( %d, %d )';
        wi_query( $query, $_SESSION[ 'user_id' ], time() );
        $issue_id = wi_insert_id( 'stamps', 'stamp_id' );

        $query = 'INSERT INTO {issues} ( issue_id, folder_id, issue_name, stamp_id ) VALUES ( %d, %d, %s, %d )';
        wi_query( $query, $issue_id, $folder_id, $name, $issue_id );

        $query = 'INSERT INTO {attr_values} ( issue_id, attr_id, attr_value ) VALUES ( %d, %d, %s )';
        foreach ( $attrs as $attr_id => $value )
            wi_query( $query, $issue_id, $attr_id, $value );

        $query = 'UPDATE {folders} SET stamp_id = %d WHERE folder_id = %d';
        wi_query( $query, $issue_id, $folder_id );

        wi_reply( array ('issue_id'=>$issue_id) );
        wi_finish_reply ();
    }

    if ( $keyword == 'RENAME ISSUE' ) {
        $issue_id = $args[ 1 ];

        $query = 'SELECT i.folder_id, i.issue_name FROM {issues} AS i';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN ) {
            $query .= ' JOIN {folders} AS f ON f.folder_id = i.folder_id';
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        }
        $query .= ' WHERE i.issue_id = %1$d';

        if ( $row = wi_query_row( $query, $issue_id, $_SESSION[ 'user_id' ] ) ) {
            $folder_id = $row[ 'folder_id' ];
            $old_name = $row[ 'issue_name' ];
        } else {
            wi_error( 312 );
        }

        $new_name = wi_string( $args[ 2 ], 80 );

        if ( $new_name == $old_name )
        {
			wi_reply( array ('result'=>'NULL') );
			wi_finish_reply ();
		}

        $query = 'INSERT INTO {stamps} ( user_id, stamp_time ) VALUES ( %d, %d )';
        wi_query( $query, $_SESSION[ 'user_id' ], time() );
        $stamp_id = wi_insert_id( 'stamps', 'stamp_id' );

        $query = 'INSERT INTO {changes} ( change_id, issue_id, attr_id, value_old, value_new )'
            . ' VALUES ( %d, %d, 0, %s, %s )';
        wi_query( $query, $stamp_id, $issue_id, $old_name, $new_name );

        $query = 'UPDATE {issues} SET issue_name = %s, stamp_id = %d WHERE issue_id = %d';
        wi_query( $query, $new_name, $stamp_id, $issue_id );

        $query = 'UPDATE {folders} SET stamp_id = %d WHERE folder_id = %d';
        wi_query( $query, $stamp_id, $folder_id );

        wi_reply( array ('stamp_id'=>$stamp_id ));
        wi_finish_reply ();
    }

    if ( $keyword == 'SET VALUE' ) {
        $issue_id = $args[ 1 ];

        $query = 'SELECT i.folder_id, f.type_id, f.project_id, i.stamp_id FROM {issues} AS i'
            . ' JOIN {folders} AS f ON f.folder_id = i.folder_id';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN )
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        $query .= ' WHERE i.issue_id = %1$d';

        if ( $row = wi_query_row( $query, $issue_id, $_SESSION[ 'user_id' ] ) ) {
            $folder_id = $row[ 'folder_id' ];
            $type_id = $row[ 'type_id' ];
            $project_id = $row[ 'project_id' ];
            $old_stamp = $row[ 'stamp_id' ];
        } else {
            wi_error( 312 );
        }

        $attr_id = $args[ 2 ];

        $query = 'SELECT attr_def FROM {attr_types} WHERE attr_id = %d AND type_id = %d';
        if ( $row = wi_query_row( $query, $attr_id, $type_id ) )
            $definition = $row[ 'attr_def' ];
        else
            wi_error( 316 );

        $query = 'SELECT attr_value FROM {attr_values} WHERE issue_id = %d AND attr_id = %d';
        if ( $row = wi_query_row( $query, $issue_id, $attr_id ) )
            $old_value = $row[ 'attr_value' ];
        else
            $old_value = '';

        $new_value = $args[ 3 ];

        $new_value = wi_verify_value( $definition, $new_value, $project_id );

        if ( $new_value == $old_value )
        {
			wi_reply( array ('result'=>'NULL') );
			wi_finish_reply ();
		}

        $query = 'INSERT INTO {stamps} ( user_id, stamp_time ) VALUES ( %d, %d )';
        wi_query( $query, $_SESSION[ 'user_id' ], time() );
        $stamp_id = wi_insert_id( 'stamps', 'stamp_id' );

        if ( $old_value == '' )
            $query = 'INSERT INTO {attr_values} ( issue_id, attr_id, attr_value ) VALUES ( %1$d, %2$d, %3$s )';
        else if ( $new_value == '' )
            $query = 'DELETE FROM {attr_values} WHERE issue_id = %1$d AND attr_id = %2$d';
        else
            $query = 'UPDATE {attr_values} SET attr_value = %3$s WHERE issue_id = %1$d AND attr_id = %2$d';
        wi_query( $query, $issue_id, $attr_id, $new_value );

        $query = 'INSERT INTO {changes} ( change_id, issue_id, attr_id, value_old, value_new )'
            . ' VALUES ( %d, %d, %d, %s, %s )';
        wi_query( $query, $stamp_id, $issue_id, $attr_id, $old_value, $new_value );

        $query = 'UPDATE {issues} SET stamp_id = %d WHERE issue_id = %d';
        wi_query( $query, $stamp_id, $issue_id );

        $query = 'UPDATE {folders} SET stamp_id = %d WHERE folder_id = %d';
        wi_query( $query, $stamp_id, $folder_id );

        wi_reply( array ('stamp_id'=>$stamp_id ));
        wi_finish_reply ();
    }

    if ( $keyword == 'ADD COMMENT' ) {
        $issue_id = $args[ 1 ];

        $query = 'SELECT i.folder_id FROM {issues} AS i';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN ) {
            $query .= ' JOIN {folders} AS f ON f.folder_id = i.folder_id';
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        }
        $query .= ' WHERE i.issue_id = %1$d';

        if ( $row = wi_query_row( $query, $issue_id, $_SESSION[ 'user_id' ] ) )
            $folder_id = $row[ 'folder_id' ];
        else
            wi_error( 312 );

        $text = rtrim( $args[ 2 ] );

        if ( $text == '' )
            wi_error( 340 );

        $query = 'INSERT INTO {stamps} ( user_id, stamp_time ) VALUES ( %d, %d )';
        wi_query( $query, $_SESSION[ 'user_id' ], time() );
        $comment_id = wi_insert_id( 'stamps', 'stamp_id' );

        $query = 'INSERT INTO {comments} ( comment_id, issue_id, comment_text ) VALUES ( %d, %d, %s )';
        wi_query( $query, $comment_id, $issue_id, $text );

        $query = 'UPDATE {issues} SET stamp_id = %d WHERE issue_id = %d';
        wi_query( $query, $comment_id, $issue_id );

        $query = 'UPDATE {folders} SET stamp_id = %d WHERE folder_id = %d';
        wi_query( $query, $comment_id, $folder_id );

        wi_reply( array ('comment_id'=>$comment_id ));
        wi_finish_reply ();
    }

    if ( $keyword == 'ADD ATTACHMENT' ) {
        $issue_id = $args[ 1 ];

        if ( !isset( $_FILES[ 'file' ] ) )
            wi_error( 400 );

        $tmp_name = $_FILES[ 'file' ][ 'tmp_name' ];
        $size = $_FILES[ 'file' ][ 'size' ];

        if ( !$tmp_name || !$size )
            wi_error( 403 );

        $query = 'SELECT i.folder_id FROM {issues} AS i';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN ) {
            $query .= ' JOIN {folders} AS f ON f.folder_id = i.folder_id';
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        }
        $query .= ' WHERE i.issue_id = %1$d';

        if ( $row = wi_query_row( $query, $issue_id, $_SESSION[ 'user_id' ] ) )
            $folder_id = $row[ 'folder_id' ];
        else
            wi_error( 312 );

        $name = wi_string( $args[ 2 ], 40 );
        $description = wi_string( $args[ 3 ], 80, false );

        $query = 'INSERT INTO {stamps} ( user_id, stamp_time ) VALUES ( %d, %d )';
        wi_query( $query, $_SESSION[ 'user_id' ], time() );
        $file_id = wi_insert_id( 'stamps', 'stamp_id' );

        if ( empty( $config[ 'storage_path' ] ) || $size < $config[ 'storage_min_size' ] ) {
            $fp = fopen( $tmp_name, 'rb' );
            $data = fread( $fp, $size );
            fclose( $fp );

            $query = 'INSERT INTO {files} ( file_id, issue_id, file_name, file_size, file_data, file_descr, file_storage )'
                . ' VALUES ( %d, %d, %s, %d, %b, %s, %d )';
            wi_query( $query, $file_id, $issue_id, $name, $size, $data, $description, 0 );
        } else {
            move_uploaded_file( $tmp_name, wi_attachment_path( $file_id, true ) );

            $query = 'INSERT INTO {files} ( file_id, issue_id, file_name, file_size, file_data, file_descr, file_storage )'
                . ' VALUES ( %d, %d, %s, %d, NULL, %s, %d )';
            wi_query( $query, $file_id, $issue_id, $name, $size, $description, 1 );
        }

        $query = 'UPDATE {issues} SET stamp_id = %d WHERE issue_id = %d';
        wi_query( $query, $file_id, $issue_id );

        $query = 'UPDATE {folders} SET stamp_id = %d WHERE folder_id = %d';
        wi_query( $query, $file_id, $folder_id );

        wi_reply( array ('file_id'=>$file_id ));
        wi_finish_reply ();
    }

    if ( $keyword == 'GET ATTACHMENT' ) {
        $file_id = $args[ 1 ];

        $query = 'SELECT fl.file_size, fl.file_data, fl.file_storage FROM {files} AS fl';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN ) {
            $query .= ' JOIN {issues} AS i ON i.issue_id = fl.issue_id';
            $query .= ' JOIN {folders} AS f ON f.folder_id = i.folder_id';
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        }
        $query .= ' WHERE fl.file_id = %1$d';

        if ( $row = wi_query_row( $query, $file_id, $_SESSION[ 'user_id' ] ) ) {
            $size = $row[ 'file_size' ];
            $data = $row[ 'file_data' ];
            $storage = $row[ 'file_storage' ];
        } else {
            wi_error( 313 );
        }

        header( "Content-Type: application/octet-stream" );
        header( "Content-Length: $size" );

        if ( $storage == 0 )
            echo wi_unescape_blob( $data );
        else
            readfile( wi_attachment_path( $file_id, false ) );

        wi_finish_reply ();
    }

    if ( $keyword == 'FIND ITEM' ) {
        $item_id = $args[ 1 ];

        $query = 'SELECT i.issue_id'
            . ' FROM {issues} AS i';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN ) {
            $query .= ' JOIN {folders} AS f ON f.folder_id = i.folder_id';
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        }
        $query .= ' WHERE i.issue_id = %1$d';

        if ( $row = wi_query_row( $query, $item_id, $_SESSION[ 'user_id' ] ) )
		{
		    wi_reply( array('item_id'=>$item_id ));
            wi_finish_reply ();
		}

        $query = 'SELECT i.issue_id'
            . ' FROM {comments} AS c'
            . ' JOIN {issues} AS i ON i.issue_id = c.issue_id';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN ) {
            $query .= ' JOIN {folders} AS f ON f.folder_id = i.folder_id';
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        }
        $query .= ' WHERE c.comment_id = %1$d';

        if ( $row = wi_query_row( $query, $item_id, $_SESSION[ 'user_id' ] ) ) {
            $issue_id = $row[ 'issue_id' ];
            wi_reply( array ('issue_id'=>$issue_id) );
            wi_finish_reply ();
        }

        $query = 'SELECT i.issue_id'
            . ' FROM {files} AS fl'
            . ' JOIN {issues} AS i ON i.issue_id = fl.issue_id';
        if ( $_SESSION[ 'user_access' ] < ACCESS_ADMIN ) {
            $query .= ' JOIN {folders} AS f ON f.folder_id = i.folder_id';
            $query .= ' JOIN {rights} AS r ON r.project_id = f.project_id AND r.user_id = %2$d';
        }
        $query .= ' WHERE fl.file_id = %1$d';

        if ( $row = wi_query_row( $query, $item_id, $_SESSION[ 'user_id' ] ) ) {
            $issue_id = $row[ 'issue_id' ];
            wi_reply ( array ('issue_id'=>$issue_id));
            wi_finish_reply ();
        }

        wi_reply( array ('result'=>'NULL') );
    }
// // 	if ( $keyword == 'ADD USER')
// // 	{
// // 		$login = wi_string( $args[ 1 ], 40 );
// // 		$name = wi_string ( $args[ 2 ], 40 );
// // 		$email = wi_string ( $args[ 3 ], 40 );
// // 		$password = wi_string ( $args[ 4 ], 40 );
// // 		$query = 'INSERT INTO {users} (user_name, user_id, user_access ) values ("%s, 0, 1")';
// //         wi_query( $query, $user_name);
// //         wi_reply( array ('result'=>'NULL') );
// // 	}
    wi_error( 401 );
}
