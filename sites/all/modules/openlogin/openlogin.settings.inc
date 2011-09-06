<?php
// $Id: openlogin.settings.inc,v 1.1 2010/07/26 12:55:52 seaji Exp $

$GLOBALS['login']['settings'] = array(
  'providers' => array(
    'google' => array(
      'name' => 'Google',
      'icon' => 'http://www.google.com/favicon.ico',
      'usercalled' => 'username',
      'url_prefix' => 'https://www.google.com/accounts/o8/id',
      'url_suffix' => '',
      'website'  => 'http://www.google.ru/intl/ru/options/',
      'openid1' => false,
    ),
    'yandex' => array(
      'name' => 'Яндекс',
      'icon' => 'http://www.yandex.ru/favicon.ico',
      'usercalled' => 'username',
      'url_prefix' => 'http://openid.yandex.ru',
      'url_suffix' => '',
      'website'  => 'http://www.yandex.ru',
      'openid1' => false,
    ),
    'rambler' => array(
      'name' => 'Рамблер',
      'icon' => 'http://www.rambler.ru/favicon.ico',
      'usercalled' => 'username',
      'url_prefix' => 'http://www.rambler.ru',
      'url_suffix' => '',
      'website'  => 'http://www.rambler.ru',
      'openid1' => false,
    ),
    'lj'      => array(
      'name' => 'Живой Журнал',
      'icon' => 'http://www.livejournal.com/favicon.ico',
      'usercalled' => 'username',
      'url_prefix' => 'http://',
      'url_suffix' => '.livejournal.com/',
      'website'  => 'http://www.livejournal.com/',
      'openid1' => true,
    ),
    'live_int'=> array(
      'name' => 'Live Internet',
      'icon' => 'http://www.liveinternet.ru/favicon.ico',
      'usercalled' => 'username',
      'url_prefix' => 'http://www.liveinternet.ru/users/',
      'url_suffix' => '',
      'website'  => 'http://www.liveinternet.ru',
      'openid1' => true,
    ),
    'blogger' => array(
      'name' => 'Blogger',
      'icon' => 'http://www.blogger.com/favicon.ico',
      'usercalled' => 'username',
      'url_prefix' => 'http://',
      'url_suffix' => '.blogspot.com',
      'website'  => 'https://www.blogger.com',
      'openid1' => true,
    ),
    'flickr'  => array(
      'name' => 'Flickr',
      'icon' => 'http://www.flickr.com/favicon.ico',
      'usercalled' => 'username',
      'url_prefix' => 'http://www.flickr.com/photos/',
      'url_suffix' => '',
      'website'  => 'http://www.flickr.com',
      'openid1' => false,
    ),
    'yahoo'   => array(
      'name' => 'Yahoo',
      'icon' => 'http://www.yahoo.com/favicon.ico',
      'usercalled'=> 'yahoo id',
      'url_prefix' => 'http://me.yahoo.com/',
      'url_suffix' => '',
      'website'  => 'http://www.yahoo.com',
      'openid1' => false,
    ),
    'windows' => array(
      'name' => 'Windows Live ID',
      'icon' => 'https://login.live-int.com/favicon.ico',
      'url_prefix' => 'https://login.live-int.com/',
      'url_suffix' => '',
      'website'  => 'https://login.live-int.com/',
      'openid1' => true,
    ),
  ),
);

$GLOBALS['login']['help_text'] = 'Есть вопросы?<br /><a href="http://openid.yandex.ru/info/faq/" target="_blank">Ответы на Яндексе</a>';

?>