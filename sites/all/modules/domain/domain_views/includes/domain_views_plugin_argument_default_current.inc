<?php
// $Id: domain_views_plugin_argument_default_current.inc,v 1.1 2009/10/05 15:17:10 agentken Exp $

/**
 * @file
 *  Domain Views plugin that contains a argument default with the current domain's id.
 *
 * @ingroup domain_views
 */

/**
 * Argument default with the current domain's id
 */
class domain_views_plugin_argument_default_current extends views_plugin_argument_default {
  function argument_form(&$form, &$form_state) {
  }

  function get_argument() {
    global $_domain;
    return $_domain['domain_id'];
  }
}

