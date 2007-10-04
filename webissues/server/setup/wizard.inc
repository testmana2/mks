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

function wizard_print()
{
    global $last_page, $page, $body, $page_titles;

    echo "<form method=\"GET\" action=\"" . SCRIPT . "\">\n";
    echo "<input type=\"hidden\" name=\"page\" value=\"$page\" />\n";

    echo "<table>\n";
    echo "<tr>\n<th colspan=\"2\">WebIssues " . VERSION . " Setup</th>\n</tr>\n";

    echo "<tr>\n<td class=\"pages\">\n<ul>\n";
    foreach ( $page_titles as $id => $title ) {
        $current = ( $id == $page ) ? " class=\"current\"" : "";
        echo "<li$current>$title</li>\n";
    }
    echo "</ul></td>\n";

    echo "<td class=\"main\">\n";
    echo $body;

    echo "</td></tr>\n";

    if ( $page != 'finished' ) {
        echo "<tr>\n<td class=\"buttons\" colspan=\"2\">\n";
        if ( $page != $last_page )
            echo "<input type=\"submit\" value=\"Next &gt;\" />\n";
        else
            echo "<input type=\"submit\" value=\"Retry\" />\n";
        echo "</td></tr>\n";
    }

    echo "</table>\n";
    echo "</form>\n";
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>WebIssues <?php echo VERSION; ?> Setup</title>
<style type="text/css">
<!--
body { font-family: sans-serif; font-size: 12pt; margin: 0px; padding: 10px; }
table { width: 100%; border: solid 1px black; border-collapse: collapse; }
th { font-weight: bold; font-size: 16pt; border: solid 1px black; text-align: left; padding: 5px 10px; color: white; background: #4466bb; }
td { vertical-align: top; }
td.pages { color: white; background: #888888; width: 160px; height: 400px; padding: 20px 10px 10px 10px; }
td.pages ul { font-size: 12pt; margin: 0px; padding: 0px 0px 0px 20px; }
td.pages li { margin: 0px 0px 10px 0px; }
li.current { font-weight: bold; font-size: 14pt; }
td.main { background: #eeeeee; padding: 20px 10px 10px 20px; }
td.main h1 { font-size: 20pt; font-weight: bold; margin: 0px 0px 20px 0px; }
td.main p { margin: 0px 0px 15px 0px; }
td.buttons { background: #cccccc; text-align: right; padding: 10px; }
td.buttons input { font-size: 12pt; background: #eeeeee; border: outset 1px #eeeeee; padding: 5px 15px; }
-->
</style>
</head>
<body>
<?php wizard_print(); ?>
</body>
</html>
