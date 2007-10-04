<?php
function wi_login ( $login, $password)
{
	unset( $_SESSION[ 'user_id' ] );
	unset( $_SESSION[ 'user_access' ] );

	$query = 'SELECT user_id, user_name, user_access FROM {users}'
		. ' WHERE user_login = BINARY %s AND user_passwd = MD5(%s)';

	$row = wi_query_row( $query, $login, $password );
	if ( ( !$row) or ($row[ 'user_access' ] == ACCESS_NONE) )
		return Null;

	$_SESSION[ 'user_id' ] = (int)$row[ 'user_id' ];
	$_SESSION[ 'user_access' ] = (int)$row[ 'user_access' ];
		
	return $row;
}
?>