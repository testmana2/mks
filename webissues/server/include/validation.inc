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

function wi_verify_definition( $definition )
{
    $info = wi_parse_definition( $definition );

    wi_verify_definition_info( $info );
}

function wi_verify_value( $definition, $value, $project_id )
{
    $info = wi_parse_definition( $definition );

    return wi_verify_value_info( $info, $value, $project_id );
}

function wi_get_default_value( $definition )
{
    $info = wi_parse_definition( $definition );

    return $info[ 'default' ];
}

function wi_parse_definition( $definition )
{
    $reg_num = '-?\d+';
    $reg_str = '"(?:\\\\["\\\\]|[^"\\\\\\n])*"';
    $reg_arr = "\{$reg_str(?:,$reg_str)*}";

    $reg_def = "/^([A-Z]+)(( ([-a-z]+)=($reg_num|$reg_str|$reg_arr))*)$/";

    if ( !preg_match( $reg_def, $definition, $parts ) )
        wi_error( 343 );

    $info[ 'type' ] = $parts[ 1 ];
    $meta = array();

    $count = preg_match_all( "/([-a-z]+)=($reg_num|$reg_str|$reg_arr)/", $parts[ 2 ], $matches );

    for ( $i = 0; $i < $count; $i++ ) {
        $value = $matches[ 2 ][ $i ];
        $letter = $value{ 0 };
        if ( $letter == '"' ) {
            $value = wi_string( wi_unquote( $value ), 80, false );
        } else if ( $letter == '{' ) {
            preg_match_all( "/$reg_str/", $value, $items );
            $value = array();
            foreach ( $items[ 0 ] as $item )
                $value[] = wi_string( wi_unquote( $item ), 80, false );
        } else {
            if ( !wi_check_integer( $value ) )
                wi_error( 343 );
            $value = (int)$value;
        }
        $meta[ $matches[ 1 ][ $i ] ] = $value;
    }

    if ( isset( $meta[ 'default' ] ) ) {
        $info[ 'default' ] = $meta[ 'default' ];
        unset( $meta[ 'default' ] );
    } else {
        $info[ 'default' ] = '';
    }

    if ( isset( $meta[ 'required' ] ) ) {
        $info[ 'required' ] = $meta[ 'required' ];
        unset( $meta[ 'required' ] );
    } else {
        $info[ 'required' ] = 0;
    }

    $info[ 'metadata' ] = $meta;

    return $info;
}

function wi_verify_definition_info( $info )
{
    switch ( $info[ 'type' ] ) {
        case 'TEXT':
            wi_verify_text_definition_info( $info );
            break;
        case 'ENUM':
            wi_verify_enum_definition_info( $info );
            break;
        case 'NUMERIC':
            wi_verify_numeric_definition_info( $info );
            break;
        case 'DATETIME':
            wi_verify_datetime_definition_info( $info );
            break;
        case 'USER':
            wi_verify_user_definition_info( $info );
            break;
        default:
            wi_error( 343 );
    }

    $required = $info[ 'required' ];
    if ( !is_int( $required ) || $required < 0 || $required > 1 )
        wi_error( 343 );

    wi_verify_value_info( $info, $info[ 'default' ], 0 );
}

function wi_verify_text_definition_info( $info )
{
    foreach ( $info[ 'metadata' ] as $meta => $value ) {
        switch ( $meta ) {
            case 'max-length':
                if ( !is_int( $value ) || $value < 1 || $value > 80 )
                    wi_error( 343 );
                break;
            default:
                wi_error( 343 );
        }
    }
}

function wi_verify_enum_definition_info( $info )
{
    if ( !wi_metadata( $info, 'items', false ) )
        wi_error( 343 );

    foreach ( $info[ 'metadata' ] as $meta => $value ) {
        switch ( $meta ) {
            case 'items':
                if ( !is_array( $value ) )
                    wi_error( 343 );
                if ( array_search( '', $value ) !== false )
                    wi_error( 343 );
                if ( count( array_unique( $value ) ) != count( $value ) )
                    wi_error( 343 );
                break;
            case 'editable':
                if ( !is_int( $value ) || $value < 0 || $value > 1 )
                    wi_error( 343 );
                break;
            default:
                wi_error( 343 );
        }
    }
}

function wi_verify_numeric_definition_info( $info )
{
    foreach ( $info[ 'metadata' ] as $meta => $value ) {
        switch ( $meta ) {
            case 'decimal':
                if ( !is_int( $value ) || $value < 0 || $value > 6 )
                    wi_error( 343 );
                break;
            case 'min-value':
            case 'max-value':
                if ( !is_string( $value ) )
                    wi_error( 343 );
                break;
            default:
                wi_error( 343 );
        }
    }

    $decimal = wi_metadata( $info, 'decimal', 0 );
    $min = wi_metadata( $info, 'min-value', false );
    $max = wi_metadata( $info, 'max-value', false );

    if ( $min !== false && !wi_check_decimal( $min, $decimal ) )
        wi_error( 342 );
    if ( $max !== false && !wi_check_decimal( $max, $decimal ) )
        wi_error( 342 );

    if ( $min !== false && $max !== false && (float)$min > (float)$max )
        wi_error( 342 );
}

function wi_verify_datetime_definition_info( $info )
{
    foreach ( $info[ 'metadata' ] as $meta => $value ) {
        switch ( $meta ) {
            case 'time':
                if ( !is_int( $value ) || $value < 0 || $value > 1 )
                    wi_error( 343 );
                break;
            default:
                wi_error( 343 );
        }
    }
}

function wi_verify_user_definition_info( $info )
{
    foreach ( $info[ 'metadata' ] as $meta => $value ) {
        switch ( $meta ) {
            case 'members':
                if ( !is_int( $value ) || $value < 0 || $value > 1 )
                    wi_error( 343 );
                break;
            default:
                wi_error( 343 );
        }
    }
}

function wi_verify_value_info( $info, $value, $project_id )
{
    $value = wi_string( $value, 80, $info[ 'required' ] );

    if ( $value == '' )
        return $value;

    switch ( $info[ 'type' ] ) {
        case 'TEXT':
            return wi_verify_text_value_info( $info, $value );
        case 'ENUM':
            return wi_verify_enum_value_info( $info, $value );
        case 'NUMERIC':
            return wi_verify_numeric_value_info( $info, $value );
        case 'DATETIME':
            return wi_verify_datetime_value_info( $info, $value );
        case 'USER':
            return wi_verify_user_value_info( $info, $value, $project_id );
        default:
            wi_error( 343 );
    }
}

function wi_verify_text_value_info( $info, $value )
{
    if ( wi_metadata( $info, 'max-length', 0 ) ) {
        if ( wi_length( $value ) > $info[ 'metadata' ][ 'max-length' ] )
            wi_error( 342 );
    }

    return $value;
}

function wi_verify_enum_value_info( $info, $value )
{
    if ( !wi_metadata( $info, 'editable', 0 ) ) {
        $items = wi_metadata( $info, 'items', array() );
        if ( array_search( $value, $items ) === false )
           wi_error( 342 );
    }

    return $value;
}

function wi_verify_numeric_value_info( $info, $value )
{
    $decimal = wi_metadata( $info, 'decimal', 0 );

    if ( !wi_check_decimal( $value, $decimal ) )
        wi_error( 342 );

    $numeric = (float)$value;

    $min = wi_metadata( $info, 'min-value', false );
    if ( $min !== false && $numeric < (float)$min )
        wi_error( 342 );

    $max = wi_metadata( $info, 'max-value', false );
    if ( $max !== false && $numeric > (float)$max )
        wi_error( 342 );

    return number_format( $numeric, $decimal, '.', '' );
}

function wi_verify_datetime_value_info( $info, $value )
{
    $reg_date = '(\d\d\d\d)-(\d\d)-(\d\d)';
    $reg_time = '(\d\d):(\d\d)';

    $time = wi_metadata( $info, 'time', 0 );
    if ( $time )
        $reg = "/^$reg_date $reg_time$/";
    else
        $reg = "/^$reg_date$/";

    if ( !preg_match( $reg, $value, $matches ) )
        wi_error( 342 );

    if ( !checkdate( $matches[ 2 ], $matches[ 3 ], $matches[ 1 ] ) )
        wi_error( 342 );

    if ( $time ) {
        if ( $matches[ 4 ] < 0 || $matches[ 4 ] > 23 || $matches[ 5 ] < 0 || $matches[ 5 ] > 59 )
            wi_error( 342 );
    }

    return $value;
}

function wi_verify_user_value_info( $info, $value, $project_id )
{
    $query = 'SELECT u.user_id FROM {users} AS u';
    if ( wi_metadata( $info, 'members', 0 ) && $project_id != 0 )
        $query .= ' JOIN {rights} AS r ON r.user_id = u.user_id AND r.project_id = %2$d';
    $query .= ' WHERE u.user_name = BINARY %1$s';

    if ( !wi_query_row( $query, $value, $project_id ) )
        wi_error( 342 );

    return $value;
}

function wi_metadata( $info, $name, $default )
{
    if ( isset( $info[ 'metadata' ][ $name ] ) )
        return $info[ 'metadata' ][ $name ];
    return $default;
}

function wi_check_decimal( $value, $decimal )
{
    if ( $decimal == 0 )
        $reg = '/^-?\d+$/';
    else
        $reg = '/^-?\d+(?:\.(\d+))?$/';
    if ( !preg_match( $reg, $value, $matches ) )
        return false;
    if ( $decimal > 0 && isset( $matches[ 1 ] ) && strlen( $matches[ 1 ] ) > $decimal )
        return false;
    return true;
}
