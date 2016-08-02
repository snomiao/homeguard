<?php
$mysqli = mysqli_connect("127.0.0.1", "root", "root", "homeguard");

if (!$mysqli) {
    echo "Error: Unable to connect to MySQL." . PHP_EOL;
    echo "Debugging errno: " . mysqli_connect_errno() . PHP_EOL;
    echo "Debugging error: " . mysqli_connect_error() . PHP_EOL;
    exit;
}
$mysqli->set_charset("utf8");
/* Create table doesn't return a resultset */

$sql = "CREATE TABLE IF NOT EXISTS homeguard(
    `id`         int unsigned auto_increment,
    `pid`                       int unsigned,
    `t_update`                      datetime,
    `data-name`                     char(32),
    `int32`                     int unsigned,
     primary key (id)
) character set = utf8;";

$mysqli->query($sql);


function insert($mysqli, $name){
    $sql = "INSERT INTO homeguard (pid, t_update, `data-name`, int32) VALUES (0, CURRENT_TIMESTAMP, '$name', 0);";
    $mysqli->query($sql);
}

function update($mysqli, $name, $int32){
    $sql = "UPDATE homeguard SET t_update=CURRENT_TIMESTAMP, int32=$int32 WHERE '$name'==`data-name`;";
    $mysqli->query($sql);
}

$sql = "SELECT count(id) count FROM homeguard where `id`=1";
$result = $mysqli->query($sql);
$row = $result->fetch_assoc();
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

$sql = "SELECT id, `data-name`, int32 FROM homeguard";
$result = $mysqli->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "id: " . $row["id"]. " - Name: " . $row["data-name"]. ", value= " . $row["int32"]. "<br>";
    }
} else {
    echo "0 results";
}

$action =   isset($_GET["action"] ) ? $_GET["action"]  :
            (isset($_POST["action"]) ? $_POST["action"] :
            "view");


function fobbiden_password_needed(){
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">

    <!--Import Google Icon Font-->
    <!--<link rel="stylesheet" href="css/fonts-material-icons.css">-->
    <link rel="stylesheet" href="http://fonts.googleapis.com/icon?family=Material+Icons">

    <!--<link rel="stylesheet" href="css/materialize.min.css">-->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.7/css/materialize.min.css">



    <!--Let browser know website is optimized for mobile-->
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>

    <style>
        *{font-family: "Microsoft YaHei"}
    </style>
</head>

<body style="background: #fcfcfc;">
<!--Import jQuery before materialize.js-->
<!--<script src="js/jquery-2.1.1.min.js"></script>-->
<script src="https://code.jquery.com/jquery-2.1.1.min.js"></script>

<!-- Compiled and minified JavaScript -->
<!--<script src="js/materialize.min.js"></script>-->
<script src="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.7/js/materialize.min.js"></script>

<header>
    <div class="contain">
        <nav>
            <div class="nav-wrapper cyan lighten-1">
                <a href="#" class="brand-logo center"><i class="material-icons left">cloud</i>雪星 - 家园卫士<i class="material-icons right">cloud</i></a>
            </div>
        </nav>
    </div>
</header>
<div class="container" style="">
    <div class="card cyan darken-4">
        <nav>
            <div class="nav-wrapper red">
                <a href="#" class="brand-logo center">请输入<?=isset($_REQUEST["key"])?"正确的":""?>访问密码</a>
            </div>
        </nav>

        <div class="card-content white">
            <form action="./" method="post">
                <div class="row">
                        <input type="password" name="key">
                        <input type="submit" class="btn right">
                </div>
            </form>
        </div>
    </div>
</body>
</html>

<?php
}

switch($action){
    case "sync":
        switch($version){
            case "2016-08-01":
                break;
        }
        //$_GET["data"]
        //$states = 
        break;
    case "post":
        $key = isset($_REQUEST["key"]) ? $_REQUEST["key"]: "";
        if(!($key === "L6fQt3z7LhCwIBWMBFIB5TTR5znL1VTq")){
            fobbiden_password_needed();
            exit;
        };

        if($action == "post"){
            if(!($数据["卧室-客厅门卫士-公牛-Z4-插座"]  == isset($_POST["卧室-客厅门卫士-公牛-Z4-插座"])  ? true : false)){
                update($mysqli, "卧室-客厅门卫士-公牛-Z4-插座",     (isset($_POST["卧室-客厅门卫士-公牛-Z4-插座"])  ? true : false));
            };
            if(!($数据["卧室-客厅门卫士-小夜灯"]        == isset($_POST["卧室-客厅门卫士-小夜灯"])        ? true : false)){
                update($mysqli, "卧室-客厅门卫士-小夜灯",           (isset($_POST["卧室-客厅门卫士-小夜灯"])        ? true : false));
            };
            if(!($数据["卧室-客厅门卫士-电脑桌-小吊扇"] == isset($_POST["卧室-客厅门卫士-电脑桌-小吊扇"]) ? true : false)){
                update($mysqli, "卧室-客厅门卫士-电脑桌-小吊扇",    (isset($_POST["卧室-客厅门卫士-电脑桌-小吊扇"]) ? true : false));
            };
            if(!($数据["卧室-客厅门卫士-电脑桌-日光灯"] == isset($_POST["卧室-客厅门卫士-电脑桌-日光灯"]) ? true : false)){
                update($mysqli, "卧室-客厅门卫士-电脑桌-日光灯",    (isset($_POST["卧室-客厅门卫士-电脑桌-日光灯"]) ? true : false));
            };
        }

    case "view":
        $key = isset($_REQUEST["key"]) ? $_REQUEST["key"] : "";
        if(!($key === "L6fQt3z7LhCwIBWMBFIB5TTR5znL1VTq")){
            fobbiden_password_needed();
            exit;
        };

        $数据 = array();
        $数据["卧室-客厅门卫士"]                        = true;
        $数据["卧室-客厅门卫士-公牛-Z4-插座"]           = false;
        $数据["卧室-客厅门卫士-小夜灯"]                 = false;
        $数据["卧室-客厅门卫士-电脑桌-小吊扇"]          = false;
        $数据["卧室-客厅门卫士-电脑桌-日光灯"]          = false;
        $数据["卧室-客厅门卫士-红外人体感应"]           = false;
        $数据["卧室-客厅门卫士-门框磁力感应"]           = false;
        $数据["卧室-客厅门卫士-门框气温"]               = "26.5℃";

        $展示 = array();

        $展示["卧室-客厅门卫士-COLORMARK"]              = $数据["卧室-客厅门卫士"] ? "green"  : "red"   ;
        $展示["卧室-客厅门卫士-CONNECTED"]              = $数据["卧室-客厅门卫士"] ? "已连接" : "未连接";
        $展示["卧室-客厅门卫士-公牛-Z4-插座"]           = $数据["卧室-客厅门卫士-公牛-Z4-插座"]  ? "checked" : "";
        $展示["卧室-客厅门卫士-小夜灯"]                 = $数据["卧室-客厅门卫士-小夜灯"]        ? "checked" : "";
        $展示["卧室-客厅门卫士-电脑桌-小吊扇"]          = $数据["卧室-客厅门卫士-电脑桌-小吊扇"] ? "checked" : "";
        $展示["卧室-客厅门卫士-电脑桌-日光灯"]          = $数据["卧室-客厅门卫士-电脑桌-日光灯"] ? "checked" : "";
        $展示["卧室-客厅门卫士-红外人体感应"]           = $数据["卧室-客厅门卫士-红外人体感应"] ? "HIGH" : "LOW";
        $展示["卧室-客厅门卫士-红外人体感应-COLORMARK"] = $数据["卧室-客厅门卫士-红外人体感应"] ? "red" : "blue";
        $展示["卧室-客厅门卫士-门框气温"]               = $数据["卧室-客厅门卫士-门框气温"] OR "无数据";
        $展示["卧室-客厅门卫士-门框气温-COLORMARK"]     = $数据["卧室-客厅门卫士-门框气温"] ? "cyan" : "red";
        $展示["卧室-客厅门卫士-门框磁力感应"]           = $数据["卧室-客厅门卫士-门框磁力感应"] ? "HIGH" : "LOW";
        $展示["卧室-客厅门卫士-门框磁力感应-COLORMARK"] = $数据["卧室-客厅门卫士-门框磁力感应"] ? "red" : "blue";
        $展示["卧室-阳台门卫士-COLORMARK"]              = "red";
        $展示["卧室-阳台门卫士-CONNECTED"]              = "未连接";
        $展示["卧室-阳台门卫士-地线状态"]               = "无数据";
        $展示["卧室-阳台门卫士-地线状态-COLORMARK"]     = "red";
        $展示["卧室-阳台门卫士-床-大灯"]                = "disabled";
        $展示["卧室-阳台门卫士-床-小台灯"]              = "disabled";
        $展示["卧室-阳台门卫士-床-小吊扇"]              = "disabled";
        $展示["卧室-阳台门卫士-门框气温"]               = "无数据";
        $展示["卧室-阳台门卫士-门框气温-COLORMARK"]     = "red";
        $展示["卧室-阳台门卫士-门框磁力感应"]           = "无数据";
        $展示["卧室-阳台门卫士-门框磁力感应-COLORMARK"] = "red";
        $展示["客厅-正门卫士--门框磁力感应-COLORMARK"]  = "red";
        $展示["客厅-正门卫士-CONNECTED"]                = "未连接";
        $展示["客厅-正门卫士-门框气温"]                 = "无数据";
        $展示["客厅-正门卫士-门框气温-COLORMARK"]       = "red";
        $展示["客厅-正门卫士-门框磁力感应"]             = "无数据";
        $展示["客厅-阳台门卫士-COLORMARK"]              = "red";
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">

    <!--Import Google Icon Font-->
    <!--<link rel="stylesheet" href="css/fonts-material-icons.css">-->
    <link rel="stylesheet" href="http://fonts.googleapis.com/icon?family=Material+Icons">

    <!--<link rel="stylesheet" href="css/materialize.min.css">-->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.7/css/materialize.min.css">



    <!--Let browser know website is optimized for mobile-->
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>

    <style>
        *{font-family: "Microsoft YaHei"}
    </style>
</head>

<body style="background: #fcfcfc;">
<!--Import jQuery before materialize.js-->
<!--<script src="js/jquery-2.1.1.min.js"></script>-->
<script src="https://code.jquery.com/jquery-2.1.1.min.js"></script>

<!-- Compiled and minified JavaScript -->
<!--<script src="js/materialize.min.js"></script>-->
<script src="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.7/js/materialize.min.js"></script>

<header>
    <div class="contain">
        <nav>
            <div class="nav-wrapper cyan lighten-1">
                <a href="#" class="brand-logo center"><i class="material-icons left">cloud</i>雪星 - 家园卫士<i class="material-icons right">cloud</i></a>
            </div>
        </nav>
    </div>
</header>
<div class="container" style="">
    <form id="mainform" method="post" action="?action=post">
        <!-- <input type="submit" name="submit" class="btn" value="提交"> -->
        <h2 class="card-title center">书房 · 卧室</h2>
        <div class="card cyan darken-4">
            <nav>
                <div class="nav-wrapper <?=$展示["卧室-客厅门卫士-COLORMARK"]?>">
                    <a href="#" class="brand-logo center">卧室 · 客厅门卫士</a>
                    <span class="badge white-text"><?=$展示["卧室-客厅门卫士-CONNECTED"]?><i class="material-icons right">cloud</i></span>
                </div>
            </nav>
            <div class="card-content">
                <ul class="collection with-header black-text">
                    <li class="collection-header">
                        <h5>电脑桌</h5>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            小吊扇
                            <label class="right">
                                <input onclick='$("#mainform").submit()' type="checkbox" name="卧室-客厅门卫士-电脑桌-小吊扇" <?=$展示["卧室-客厅门卫士-电脑桌-小吊扇"]?>>
                                <span class="lever"></span>
                            </label>
                        </div>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            日光灯
                            <label class="right">
                                <input onclick='$("#mainform").submit()' type="checkbox" name="卧室-客厅门卫士-电脑桌-日光灯" <?=$展示["卧室-客厅门卫士-电脑桌-日光灯"]?> >
                                <span class="lever"></span>
                            </label>
                        </div>
                    </li>
                </ul>
                <ul class="collection with-header black-text">
                    <li class="collection-header">
                        <h5>客厅门</h5>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            公牛 Z4 插座
                            <label class="right">
                                <input onclick='$("#mainform").submit()' type="checkbox" name="卧室-客厅门卫士-公牛-Z4-插座" <?=$展示["卧室-客厅门卫士-公牛-Z4-插座"]?> >
                                <span class="lever"></span>
                            </label>
                        </div>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            小夜灯
                            <label class="right">
                                <input onclick='$("#mainform").submit()' type="checkbox" name="卧室-客厅门卫士-小夜灯" <?=$展示["卧室-客厅门卫士-小夜灯"]?> >
                                <span class="lever"></span>
                            </label>
                        </div>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            红外人体感应
                            <span class="badge white-text <?=$展示["卧室-客厅门卫士-红外人体感应-COLORMARK"]?>">
                                <?=$展示["卧室-客厅门卫士-红外人体感应"]?>
                            </span>
                        </div>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            门框磁力感应
                            <span class="badge white-text <?=$展示["卧室-客厅门卫士-门框磁力感应-COLORMARK"]?>">
                                <?=$展示["卧室-客厅门卫士-门框磁力感应"]?>
                            </span>
                        </div>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            门框气温
                            <span class="badge white-text <?=$展示["卧室-客厅门卫士-门框气温-COLORMARK"]?>">
                                <?=$展示["卧室-客厅门卫士-门框气温"]?>
                            </span>
                        </div>
                    </li>
                </ul>
            </div>
        </div>
        <div class="card cyan darken-4">
            <nav>
                <div class="nav-wrapper <?=$展示["卧室-阳台门卫士-COLORMARK"]?>">
                    <a href="#" class="brand-logo center">卧室 · 阳台门卫士</a>
                    <span class="badge white-text"><?=$展示["卧室-阳台门卫士-CONNECTED"]?><i class="material-icons right">cloud</i></span>
                </div>
            </nav>
            <div class="card-content">
                <ul class="collection with-header black-text">
                    <li class="collection-header">
                        <h5>床</h5>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            小吊扇
                            <label class="right">
                                <input onclick='$("#mainform").submit()' type="checkbox" name="卧室-阳台门卫士-床-小吊扇" <?=$展示["卧室-阳台门卫士-床-小吊扇"]?>>
                                <span class="lever"></span>
                            </label>
                        </div>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            大灯
                            <label class="right">
                                <input onclick='$("#mainform").submit()' type="checkbox" name="卧室-阳台门卫士-床-大灯" <?=$展示["卧室-阳台门卫士-床-大灯"]?> >
                                <span class="lever"></span>
                            </label>
                        </div>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            小台灯
                            <label class="right">
                                <input onclick='$("#mainform").submit()' type="checkbox" name="卧室-阳台门卫士-床-小台灯" <?=$展示["卧室-阳台门卫士-床-小台灯"]?> >
                                <span class="lever"></span>
                            </label>
                        </div>
                    </li>
                </ul>
                <ul class="collection with-header black-text">
                    <li class="collection-header">
                        <h5>阳台</h5>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            地线状态
                            <span class="badge white-text <?=$展示["卧室-阳台门卫士-地线状态-COLORMARK"]?>">
                                <?=$展示["卧室-阳台门卫士-地线状态"]?>
                            </span>
                        </div>
                    </li>
                </ul>
                <ul class="collection with-header black-text">
                    <li class="collection-header">
                        <h5>阳台 - 卧室门</h5>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            门框磁力感应
                            <span class="badge white-text <?=$展示["卧室-阳台门卫士-门框磁力感应-COLORMARK"]?>">
                                <?=$展示["卧室-阳台门卫士-门框磁力感应"]?>
                            </span>
                        </div>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            门框气温
                            <span class="badge white-text <?=$展示["卧室-阳台门卫士-门框气温-COLORMARK"]?>">
                                <?=$展示["卧室-阳台门卫士-门框气温"]?>
                            </span>
                        </div>
                    </li>
                </ul>
            </div>
        </div>

        <h2 class="card-title center">客厅</h2>
        <div class="card cyan darken-4">
            <nav>
                <div class="nav-wrapper <?=$展示["客厅-阳台门卫士-COLORMARK"]?>">
                    <a href="#" class="brand-logo center">客厅 · 正门卫士</a>
                    <span class="badge white-text"><?=$展示["客厅-正门卫士-CONNECTED"]?><i class="material-icons right">cloud</i></span>
                </div>
            </nav>
            <div class="card-content">
                <ul class="collection with-header black-text">
                    <li class="collection-header">
                        <h5>正门</h5>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            门框磁力感应
                            <span class="badge white-text <?=$展示["客厅-正门卫士--门框磁力感应-COLORMARK"]?>">
                                <?=$展示["客厅-正门卫士-门框磁力感应"]?>
                            </span>
                        </div>
                    </li>
                    <li class="collection-item">
                        <div class="switch">
                            门框气温
                            <span class="badge white-text <?=$展示["客厅-正门卫士-门框气温-COLORMARK"]?>">
                                <?=$展示["客厅-正门卫士-门框气温"]?>
                            </span>
                        </div>
                    </li>
                </ul>
            </div>
        </div>
    </form>
</div>
</body>
</html>
<?php
    break;
}
?>