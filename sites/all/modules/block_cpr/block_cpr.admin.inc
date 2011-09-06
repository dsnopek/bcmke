<?php
// $Id: block_cpr.admin.inc,v 1.2.2.3 2010/01/27 17:16:45 njt1982 Exp $

/**
 * @file
 *  Block CPR Admin Functions
 */


/**
 * Overview form
 */
function block_cpr_overview_form() {
  // Get the settings and roles
  $settings = variable_get('block_cpr_settings', array());

  $rows = array();

  // If there are no settings, one full row informing the user there are no blocks
  if (empty($settings)) {
    $rows[] = array(
      array('data' => t('No CPR Blocks configured'), 'colspan' => 3),
    );
  }
  else {
    // Get the system roles
    $roles = user_roles();

    // For each setting, display the delta, the enabled roles as an item list and finally some operations.
    foreach ($settings as $delta => $s) {
      $enabled_roles = array();
      foreach ($s as $role_id => $rs) {
        $enabled_roles[] = check_plain($roles[$role_id]);
      }

      $ops = array();
      $ops[] = l(t('Edit'), 'admin/build/block/cpr/edit/'. $delta);
      $ops[] = l(t('Delete'), 'admin/build/block/cpr/delete/'. $delta);

      $rows[] = array(
        check_plain($delta),
        theme('item_list', $enabled_roles),
        implode(' | ', $ops),
      );
    }
  }

  // Return the resulting rows as a table
  return theme('table', array(t('Delta'), t('Roles Enabled'), t('Ops')), $rows, array('id' => 'block_cpr_overview'));
}


/**
 * The individual block settings form. This isused for adding AND editing
 */
function block_cpr_block_settings_form($form_state, $delta = NULL) {
  if ($delta) {
    // Get the settings
    $settings = variable_get('block_cpr_settings', array());

    // Reset the breadcrumb
    $breadcrumb = drupal_get_breadcrumb();
    $breadcrumb[] = l(t('CPR'), 'admin/build/block/cpr');
    drupal_set_breadcrumb($breadcrumb);
  }

  // Get the site roles
  $roles = user_roles();

  // If we have any settings for this delta (eg edit rather then add)...
  if (isset($settings[$delta])) {
    // The first set of roles ID's in the list are the ordered ones from the settings
    $ordered_roles = array_keys($settings[$delta]);

    // Then we merge in the "difference" between the full role list and the ones enabled...
    $ordered_roles = array_merge($ordered_roles, array_diff(array_keys($roles), array_keys($settings[$delta])));
  }
  else {
    // ... otherwise just make a list of site roles
    $ordered_roles = array_keys($roles);
  }

  $form = array();
  $form['#tree'] = TRUE;

  if (isset($delta)) {
    $form['delta_text'] = array(
      '#title' => t('Delta'),
      '#type' => 'item',
      '#description' => t('This is now read-only and cannot be changed'),
      '#value' => '<code>'. $delta .'</code>',
    );
    $form['delta'] = array('#type' => 'value', '#value' => $delta);
  }
  else {
    $form['delta'] = array(
      '#type' => isset($delta) ? 'item' : 'textfield',
      '#title' => t('Delta'),
      '#description' => t('The block delta is used "internally" to uniquely identify the block'),
      '#required' => TRUE,
    );
  }

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role Content'),
  );

  // Each role gets its own row to determin if it isenabled, its weight and its content.
  foreach ($ordered_roles as $rid) {
    $form['roles']['#role_enabled'][$rid] = isset($settings[$delta][$rid]);
    $status = isset($settings[$delta][$rid]) ? 'enabled' : 'disabled';

    $form['roles']['role'][$rid] = array(
      '#type' => 'markup',
      '#value' => $roles[$rid]
    );

    $form['roles']['enabled'][$rid] = array(
      '#type' => 'checkbox',
      '#attributes' => array('class' => 'role-status-element role-status-element-'. $status),
      '#default_value' => isset($settings[$delta][$rid]),
    );

    $form['roles']['weight'][$rid] = array(
      '#type' => 'weight',
      '#attributes' => array('class' => 'role-weight-element role-weight-element-'. $status),
      '#default_value' => isset($settings[$delta][$rid]['weight']) ? $settings[$delta][$rid]['weight'] : 0
    );

    $form['roles']['content'][$rid] = array(
      '#type' => 'fieldset',
      '#title' => t('Content'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['roles']['content'][$rid]['body'] = array(
      '#type' => 'textarea',
      '#default_value' => isset($settings[$delta][$rid]['content']['body']) ? $settings[$delta][$rid]['content']['body'] : ''
    );

    $form['roles']['content'][$rid]['format'] = filter_form(
      isset($settings[$delta][$rid]['content']['format']) ? $settings[$delta][$rid]['content']['format'] : FILTER_FORMAT_DEFAULT,
      NULL,
      array('roles', 'content', $rid, 'format')
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  $form['#redirect'] = 'admin/build/block/cpr';
  return $form;
}


/**
 * Theme handler for the block settings
 */
function theme_block_cpr_block_settings_form($form) {
  $output = '';

  // Get a reference to the roles section - cleaner code
  $roles = &$form['roles'];
  $rows = array();

  /**
   * Enabled Roles
   */

  // Add a header row for the enabled blocks
  $rows[] = array(
    'class' => 'status status-enabled',
    'data' => array(array('colspan' => 4, 'class' => 'status', 'data' => t('Enabled'))),
  );

  // Get the children - do it ones rather than for each foreach loop...
  $children = element_children($roles['role']);

  // Tally up a counter for the statuses - this is used for the default populated section row
  $status_count = array('enabled' => 0, 'disabled' => 0);
  foreach ($children as $rid) {
    $status_count[$roles['#role_enabled'][$rid] ? 'enabled' : 'disabled']++;
  }

  // Add a row for empty status - this has to come first...
  $rows[] = array(
    'class' => 'status-message status-enabled-message '. ($status_count['enabled'] ? 'status-populated' : 'status-empty'),
    'data' => array(array('colspan' => 4, 'data' => '<em>'. t('No roles enabled') .'</em>' )),
  );

  // For each enabled role element - render a table row
  foreach ($children as $rid) {
    if ($roles['#role_enabled'][$rid]) {
      // Enabled row found - disable the flag
      $no_roles = FALSE;

      // Define the row
      $rows[] = array(
        'class' => 'draggable',
        'data' => array(
          array('data' => drupal_render($roles['role'][$rid]), 'class' => 'rolecell'),
          array('data' => drupal_render($roles['weight'][$rid])),
          array('data' => drupal_render($roles['enabled'][$rid])),
          array('data' => drupal_render($roles['content'][$rid]), 'class' => 'contentcell'),
        ),
      );
    }
  }

  /**
   *  Disabled Rows
   */

  // Add a header row for the disabled blocks
  $rows[] = array(
    'class' => 'status status-disabled',
    'data' => array(array('colspan' => 4, 'class' => 'status', 'data' => t('Disabled'))),
  );

  // Add a row for empty disabled status - this has to come first
  $rows[] = array(
    'class' => 'status-message status-disabled-message '. ($status_count['disabled'] ? 'status-populated' : 'status-empty'),
    'data' => array(array('colspan' => 4, 'data' => '<em>'. t('No roles disabled') .'</em>' )),
  );

  // For each disabled role element - render a table row
  foreach ($children as $rid) {
    if (!$roles['#role_enabled'][$rid]) {
      // Define the row
      $rows[] = array(
        'class' => 'draggable',
        'data' => array(
          drupal_render($roles['role'][$rid]),
          drupal_render($roles['weight'][$rid]),
          drupal_render($roles['enabled'][$rid]),
          drupal_render($roles['content'][$rid]),
        ),
      );
    }
  }



  // Render the table into the roles fieldset value
  $roles['#value'] = theme('table', array(t('Role'), t('Weight'), t('Content')), $rows, array('id' => 'block_cpr'));

  drupal_add_css(drupal_get_path('module', 'block_cpr') .'/block_cpr.admin.css', 'module', 'all', FALSE);
  drupal_add_js(drupal_get_path('module', 'block_cpr') .'/block_cpr.admin.js');

  foreach (array('enabled', 'disabled') as $status) {
    drupal_add_tabledrag('block_cpr', 'match', 'sibling', 'role-status-element', 'role-status-element-'. $status);
    drupal_add_tabledrag('block_cpr', 'order', 'sibling', 'role-weight-element', 'role-weight-element-'. $status);
  }

  // Render the entire form out
  return drupal_render($form);
}


/**
 * Validation handler for the block settings form.
 */
function block_cpr_block_settings_form_validate($form, &$form_state, $delta = NULL) {
  // Does the delta contain any non lowercase alpha, numberic or underscore characters? If so - tell the user not to use them
  if (preg_match('|[^a-z0-9_]|', $form_state['values']['delta'])) {
    form_set_error('delta', t('The delta must be made of lowercase letters, numbers and underscores only.'));
  }

  // Has the user enabled any roles? We need at least one for this block to have a point.
  if (count(array_filter($form_state['values']['roles']['enabled'])) == 0) {
    form_set_error('roles][enabled', t('You must enable at least one role'));
  }
}


/**
 * Submit handler for the block settings form
 */
function block_cpr_block_settings_form_submit($form, &$form_state, $delta = NULL) {
  // Get the existing settings
  $settings = variable_get('block_cpr_settings', array());

  // Prepare a new sub-settings variable
  $s = array();

  // Foreach enabled role, create a sub-settins entry containing the weight and the content (body & format) settings
  foreach ($form_state['values']['roles']['enabled'] as $rid => $enabled) {
    if ($enabled) {
      $s[$rid] = array(
        'weight' => $form_state['values']['roles']['weight'][$rid],
        'content' => $form_state['values']['roles']['content'][$rid],
      );
    }
  }

  // Sort the sub-settings using the custom function (sorts by weight and then an alpha sort on the role name)
  uasort($s, '_block_cpr_sort_blocks');

  // Place the new sub-settings into the settings table
  $settings[$form_state['values']['delta']] = $s;

  // Save settings
  variable_set('block_cpr_settings', $settings);
}


/**
 * Custom sort function for storing the block's enabled role settings in the correct order.
 */
function _block_cpr_sort_blocks($a, $b) {
  if ($a['weight'] > $b['weight']) return 1;
  elseif ($a['weight'] < $b['weight']) return -1;

  return strcasecmp($a, $b);
}


/**
 * Delete confirm form
 */
function block_cpr_block_delete_confirm_form($delta) {
  // Get the settings
  $settings = variable_get('block_cpr_settings', array());

  // Make sure the delta being deleted exists
  if (!isset($settings[$delta])) {
    drupal_not_found();
    exit;
  }

  $form = array();
  $form['delta'] = array('#type' => 'value', '#value' => $delta);

  return confirm_form($form, t('Really delete @delta', array('@delta' => $delta)), 'admin/build/block/cpr');
}


/**
 * Submit handler for the confirm delete form
 */
function block_cpr_block_delete_confirm_form_submit($form, &$form_state) {
  $settings = variable_get('block_cpr_settings', array());
  unset($settings[$form_state['values']['delta']]);
  variable_set('block_cpr_settings', $settings);
  return 'admin/build/block/cpr';
}
