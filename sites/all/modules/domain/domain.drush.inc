<?php

// $Id: domain.drush.inc,v 1.1.2.2 2010/03/01 03:00:09 agentken Exp $

/**
 * @file
 * Drush commands for Domain Access.
 */

/**
 * Implement hook_drush_command().
 */
function domain_drush_command() {
  $items = array();

  $items['domain-list'] = array(
    'description' => 'List active domains for the site.',
  );

  return $items;
}

/**
 * Implement hook_drush_help().
 */
function domain_drush_help($section) {
  $items = domain_drush_command();
  $name = str_replace('domain:', '', $section);
  if (isset($items[$name])) {
    return dt($items[$name]['description']);
  }
}

/**
 * Show the domain list.
 */
function drush_domain_list() {
  $domains = domain_domains();
  $header = array(
    'sitename' => dt('sitename'),
    'subdomain' => dt('domain'),
    'scheme' => dt('scheme'),
    'valid' => dt('active'),
    'domain_id' => dt('domain_id'),
  );
  $rows = array(array_values($header));
  foreach ($domains as $domain) {
    $row = array();
    foreach ($header as $key => $name) {
      $row[] = $domain[$key];
    }
    $rows[] = $row;
  }
  drush_print_table($rows, TRUE);
}
