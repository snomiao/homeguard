<?php
/*
 * 2016-8-2
 * SnowstarCyan( SnowstarCyan@gmail.com )
 */

require_once("function.php");
if( $row["count"] == 0){
    insert($mysqli, "卧室-客厅门卫士");
    insert($mysqli, "卧室-客厅门卫士-公牛-Z4-插座");
    insert($mysqli, "卧室-客厅门卫士-小夜灯");
    insert($mysqli, "卧室-客厅门卫士-电脑桌-小吊扇");
    insert($mysqli, "卧室-客厅门卫士-电脑桌-日光灯");
    insert($mysqli, "卧室-客厅门卫士-红外人体感应");
    insert($mysqli, "卧室-客厅门卫士-门框磁力感应");
    insert($mysqli, "卧室-客厅门卫士-门框气温");
}

class homeguard
{
    private static $mysqli;
    private static function init_mysqli(){
        if ( isset(self::$mysqli) ) return;

        self::$mysqli = mysqli_connect("127.0.0.1", "root", "root", "homeguard");

        if (!self::$mysqli) {
            include("404.php");
            exit;
        }

        self::$mysqli->set_charset("utf8");
    }
    private static function query($sql){
        self::$mysqli->query($sql);

        if (!self::$mysqli) {

            include("Error.php");
            exit;
        }
    }

    public static function xxx(){

    }

    private static function insert($mysqli, $name){
        $sql = "INSERT INTO homeguard (pid, t_update, `data-name`, int32) VALUES (0, CURRENT_TIMESTAMP, '$name', 0);";
        self::query($sql);
    }

    function update($mysqli, $name, $int32){
        $sql = "UPDATE homeguard SET t_update=CURRENT_TIMESTAMP, int32=$int32 WHERE '$name'==`data-name`;";
        self::query($sql);
    }
    public function __construct()
    {
        self::init_mysqli();
    }
}
