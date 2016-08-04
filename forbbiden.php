<?php

function forbidden_password_needed(){
    define("TITLE", "访问限制");
    include("header.php");
    ?>
    <div class="container" style="">
        <div class="card cyan darken-4">
            <nav>
                <div class="nav-wrapper red">
                    <a href="#" class="brand-logo center">请输入<?=isset($_REQUEST["key"]) ? "正确的" : ""?>访问密码</a>
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
    </div>
    <?php
    include("footer.php");
    exit();
}