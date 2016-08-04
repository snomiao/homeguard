<?php
/**
 * Created by PhpStorm.
 * User: 99759
 * Date: 2016/8/4 0004
 * Time: 18:01
 */

$v = $_GET["test"];
var_dump($v);
print("<BR>");
var_dump(hexdec($v));
print("<BR>");
$v = base_convert($v, 16, 16);
var_dump($v);
print("<BR>");
$v = base_convert($v, 10, 16);
var_dump($v);