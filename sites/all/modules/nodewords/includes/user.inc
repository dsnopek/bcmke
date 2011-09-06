<?php
// $Id: user.inc,v 1.1.4.10 2010/05/19 20:20:28 kiam Exp $

/**
 * @file
 * Integration file for user.module.
 */

/**
 * Implements hook_nodewords_type_id().
 */
function user_metatags_type(&$result, $arg) {
  $bool = (
    $arg[0] == 'user' &&
    isset($arg[1]) && is_numeric($arg[1])
  );
  if ($bool) {
    $result['type'] = NODEWORDS_TYPE_USER;
    $result['id'] = $arg[1];
  }
}
