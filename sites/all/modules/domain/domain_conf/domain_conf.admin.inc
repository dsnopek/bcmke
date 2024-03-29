<?php
// $Id: domain_conf.admin.inc,v 1.14.2.4 2010/04/29 21:31:23 agentken Exp $

/**
 * @file
 * Domain manager configuration options.
 *
 * Provides admin screens for configuration overrides.
 * @ingroup domain_conf
 */

/**
 * The domain conf page callback router.
 *
 * @param $domain
 *   The $domain object created by domain_lookup().
 * @return
 *   The appropriate form or an error message.
 */
function domain_conf_page($domain) {
  if ($domain == -1) {
    return t('Invalid page requested.');
  }

  $output = theme('domain_conf_reset', $domain);
  if ($domain['domain_id'] > 0) {
    drupal_set_title(t('!site : Domain site settings', array('!site' => $domain['sitename'])));
    return $output . drupal_get_form('domain_conf_form', $domain);
  }
  else if ($domain['domain_id'] == 0) {
    return $output . drupal_get_form('domain_conf_default', $domain);
  }
}

/**
 * Custom form to generate domain-specific site settings.
 *
 * The items on this form are taken from hook_domainbatch() and hook_domainconf().
 * See the API for more information.
 *
 * @param $domain
 *   The domain currently being updated.
 */
function domain_conf_form($form_state, $domain) {
  $form = array();
  $batch = module_invoke_all('domainbatch');
  $settings = array();
  $data = db_result(db_query("SELECT settings FROM {domain_conf} WHERE domain_id = %d", $domain['domain_id']));
  if (!empty($data)) {
    $settings = domain_unserialize($data);
  }

  $default_group = t('Site configuration');
  foreach ($batch as $key => $action) {
    if ($action['#domain_action'] != 'domain_conf') {
      continue;
    }
    if ($action['#form']['#type'] == 'select') {
      $action['#form']['#options'] = array('domain-conf-ignore' => t('Use primary domain settings')) + $action['#form']['#options'];
    }
    $group = isset($action['#group']) ? $action['#group'] : $default_group;
    if (!isset($form[$group])) {
      $form[$group] = array(
        '#type' => 'fieldset',
        '#title' => $group,
        '#collapsible' => TRUE,
      );
    }
    $form[$group][$key] = $action['#form'];
    $form[$group][$key]['#default_value'] = isset($settings[$key]) ? $settings[$key] : $action['#system_default'];
    // Change the path for the frontpage.
    if ($key == 'site_frontpage') {
      global $base_url;
      $prefix = $base_url .'/';
      $_path = parse_url($prefix);
      $str = $_path['host'];
      $fix = preg_replace("/$str/", $domain['subdomain'], $prefix, 1);
      $form[$default_group]['site_frontpage']['#field_prefix'] = $fix;
    }
  }
  $form['domain_id'] = array('#type' => 'value', '#value' => $domain['domain_id']);
  // Site name must be edited at the domain creation screen.
  $form[$default_group]['site_name'] = array(
    '#disabled' => TRUE,
    '#title' => t('Site name'),
    '#description' => t('The name of this web site, as entered in the <a href="!url">domain-specific settings</a>.', array('!url' => url('admin/build/domain/edit/'. $domain['domain_id']))),
    '#type' => 'textfield',
    '#default_value' => $domain['sitename'],
    '#weight' => -100,
  );

  // Locale module is a little tricky, so handle it properly.
  $str = t('Language settings');
  if (isset($form[$str]['language_default']) && !isset($settings['language_default'])) {
    $form[$str]['language_default']['#default_value'] = NULL;
  }

  // Grab any extra elements defined by other modules.
  $extra = domain_conf_api(TRUE, $settings);
  // Merge the $extra and $form arrays.
  $form = array_merge($form, $extra);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save domain settings'),
    '#weight' => 10
  );

  return $form;
}

/**
 * Special configuration options for the main domain.
 *
 * @param $domain
 *   The $domain object for the default domain.
 * @return
 *   A $form array according to the FormsAPI, if unique configuration is possible.
 */

function domain_conf_default($domain) {
  drupal_set_title(t('!site : Domain site settings', array('!site' => $domain['sitename'])));
  $form = array();
  // Grab any extra elements defined by other modules.
  $extra = domain_conf_api();
  if (!empty($extra)) {
    // Convert the $extra array to the $form array.
    $form = $extra;
    $form['domain_conf_message'] = array(
      '#type' => 'markup',
      '#value' => t('<p>The following custom settings may be applied to the main domain.  These options are specific to the Domain module and do not have standard configuration pages.</p>'),
      '#weight' => -100,
    );
    // Domain information, for saving.
    $form['domain_id'] = array('#type' => 'value', '#value' => $domain['domain_id']);
    // Submit functions
    $form['#submit'][] = 'domain_conf_form_submit';
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save domain settings'),
      '#weight' => 10
    );
  }
  else {
    $form['domain_conf_message'] = array('#type' => 'markup', '#value' => t('There are no custom domain settings to configure.'));
  }
  return $form;
}

/**
 * FormsAPI for domain_conf_form().
 */
function domain_conf_form_submit($form, &$form_state) {
  $new_settings = array();
  // Throw away what we don't need.
  $ignore = array('form_token', 'form_id', 'form_build_id', 'op', 'submit', 'domain_id');
  foreach ($form_state['values'] as $key => $value) {
    if (in_array($key, $ignore) || (empty($value) && $value === 0)) {
      continue;
    }
    $new_settings[$key] = $value;
  }
  // INSERT or UPDATE?
  $result = db_fetch_object(db_query("SELECT domain_id, settings FROM {domain_conf} WHERE domain_id = %d", $form_state['values']['domain_id']));
  if (!empty($result->settings)) {
    $settings = domain_unserialize($result->settings);
    $merged_settings = array_merge($settings, $new_settings);
    $sql = "UPDATE {domain_conf} SET settings = %b WHERE domain_id = %d";
    db_query($sql, serialize($merged_settings), $form_state['values']['domain_id']);
  }
  else {
    $sql = "INSERT INTO {domain_conf} (domain_id, settings) VALUES (%d, %b)";
    db_query($sql, $form_state['values']['domain_id'], serialize($new_settings));
  }
  drupal_set_message(t('Domain options saved successfully.'));
  // Clear the cache.
  cache_clear_all();
}

/**
 * Resets configuration settings by removing the domain row from {domain_conf}.
 *
 * @param $domain
 *   The $domain object created by domain_lookup().
 * @return
 *   A confirmation form.
 */

function domain_conf_reset($domain) {
  if ($domain == -1) {
    return t('Invalid page requested.');
  }
  return drupal_get_form('domain_conf_reset_form', $domain);
}

/**
 * FormsAPI for resetting a domain configuration.
 *
 * @param $domain
 *   The $domain object for the selected domain.
 * @return
 *   Themed HTML form.
 */

function domain_conf_reset_form($form_state, $domain) {
  $extra['domain_id'] = array('#type' => 'value', '#value' => $domain['domain_id']);
  $form = confirm_form($extra, t('Are you sure you wish to reset the settings for %name?', array('%name' => $domain['sitename'])), 'admin/build/domain/conf/'. $domain['domain_id'], t('Submitting this form will restore default settings for this domain.'));
  return $form;
}

/**
 * FormsAPI for domain_conf_reset_form().
 */
function domain_conf_reset_form_submit($form, &$form_state) {
  db_query("DELETE FROM {domain_conf} WHERE domain_id = %d", $form_state['values']['domain_id']);
  drupal_set_message(t('Domain configuration settings have been reset.'));
  $form_state['redirect'] = 'admin/build/domain/conf/'. $form_state['values']['domain_id'];
  // Clear the cache.
  cache_clear_all();
}

/**
 * Theme a message at the top of domain configuration pages.
 *
 * @param $domain
 *   The $domain object for the selected domain.
 * @return
 *   Themed HTML messages.
 */
function theme_domain_conf_reset($domain) {
  $output = '';
  $output .= '<p>'. t('These settings will replace or supplement your default site settings when %name is the active domain.', array('%name' => $domain['sitename'])) .'</p>';
  $data = db_fetch_array(db_query("SELECT settings FROM {domain_conf} WHERE domain_id = %d", $domain['domain_id']));
  if (!empty($data)) {
    $output .= '<p>'. t('You may <a href="!url">erase these settings</a> to restore the default behavior.', array('!url' => url('admin/build/domain/conf-reset/'. $domain['domain_id']))) .'</p>';
  }
  return $output;
}
