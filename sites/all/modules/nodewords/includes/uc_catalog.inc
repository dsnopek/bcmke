<?php
// $Id: uc_catalog.inc,v 1.1.2.6 2010/05/19 20:20:28 kiam Exp $

/**
 * @file
 * Integration file for uc_catalog.module.
 */

/**
 * Implements hook_nodewords_type_id().
 */
function uc_catalog_nodewords_type_id(&$result, $arg) {
  $bool = (
    $arg[0] == 'catalog' &&
    isset($arg[1]) && is_numeric($arg[1])
  );
  if ($bool) {
    $result['type'] = NODEWORDS_TYPE_TERM;
    $result['id'] = $arg[1];
  }
}
