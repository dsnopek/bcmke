// $Id: README.txt,v 1.1 2010/07/26 12:55:52 seaji Exp $

DESCRIPTION
-----------
Open login module adds handy widget to openID login form

INSTALL
-------
1) Copy the module folder to the modules folder in your installation.
2) Enable the module using Administer -> Modules (/admin/build/modules)


CONFIGURATION
-------------
List openID providers in openlogin.settings.inc file
Example:

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

name - display name in the form
icon - icon file (display in 16x16)

if openid1 == true
then no user name used in link formation

if openid1 == false
then user name is used in link formation in the format in:

url_prefix + user_name + url_suffix


THEME
-----------
Use css in your theme
