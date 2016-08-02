<?php
/**
 * Created by PhpStorm.
 * User: 99759
 * Date: 2016/8/1 0001
 * Time: 8:43
 */

// connect to:

function getor(&$var, $default_var){
    if (!isset($var)){
        $var = $default_var;
        return true;
    } else {
        return false;
    }
}
