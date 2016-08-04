<?php


require("homeguard.php");
require_once("function.php");

$action =   isset($_GET["action"] ) ? $_GET["action"]  :
            (isset($_POST["action"]) ? $_POST["action"] :
            "view");

switch($action){
    case "sync":
        switch($version){
            case "2016-08-01":
                $states = isset($数据["data_states"]) ? $数据["data_states"] : 0;
                print($states);
                break;
            default:
                break;
        }
        //$_GET["data"]
        //$states = 
        break;
    case "post":
        $key = isset($_REQUEST["key"]) ? $_REQUEST["key"]: "";
        if(!($key === "L6fQt3z7LhCwIBWMBFIB5TTR5znL1VTq")){
            forbidden_password_needed();
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
            forbidden_password_needed();
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

<? include("header.php"); ?>
<div class="container">
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
<?php
    include("footer.php");
    break;
}
?>