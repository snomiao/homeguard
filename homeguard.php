<?php
/*
 * 2016-8-2
 * SnowstarCyan( SnowstarCyan@gmail.com )
 */

class homeguard
{
    private static $mysqli;
    public static function init_mysqli(){
        if ( isset(self::$mysqli) ) return;

        self::$mysqli = mysqli_connect("127.0.0.1", "root", "root", "homeguard");

        if (!self::$mysqli) {
            include("404.php");
            exit;
        }

        self::$mysqli->set_charset("utf8");
    }
    private static function homeguard(){
        self->init_mysqli();

    }
    function
}
