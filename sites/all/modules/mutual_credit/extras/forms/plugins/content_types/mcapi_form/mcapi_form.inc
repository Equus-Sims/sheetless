<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Transaction form'),
  'description' => t('Display the transfer form.'),
  'required context' => new ctools_context_required(t('User'), 'user'),
  'category' => t('Form'),
  //'edit form' => 'mcapi_form_content_type_settings', // A function that will return the settings form for the pane.
  'defaults' => array( // Array of defaults for the settings form.
    'show_balance' => TRUE,
    'mcapi_form_id' => '1stparty'
  )
);

function mcapi_forms_mcapi_form_content_type_edit_form($form, &$form_state) {

  $conf = $form_state['conf'];
  $form['show_balance'] = array(
    '#title' => t('Show balance in header'),
    '#type' => 'checkbox',
    '#default_value' => $form_state['conf']['show_balance'],
  );
  foreach (mcapi_forms_load() as $key => $settings) {
    if (!$settings->status) continue;
    $options[$key] = filter_xss($settings->data['experience']['title']);
  }
  $form['mcapi_form_id'] = array(
    '#title' => t('Transaction form'),
    '#description' => t('See !link', array('!link' => l('admin/accounting/workflow/forms', 'admin/accounting/workflow/forms'))),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $form_state['conf']['mcapi_form_id'],
  );
  return $form;
}

/**
 * Submit function
 * save the settings
 */
function mcapi_forms_mcapi_form_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['show_balance'] = $form_state['values']['show_balance'];
  $form_state['conf']['mcapi_form_id'] = $form_state['values']['mcapi_form_id'];
}

/**
 * Render callback.
 */
function mcapi_forms_mcapi_form_content_type_render($subtype, $conf, $args, $context) {
  if (empty($context->data)) {
    return;
  }
  $title = drupal_get_title();
  $form = mcapi_get_form($conf['mcapi_form_id']);
  $form_title = drupal_get_title();
  drupal_set_title($title);

  $title = '<span class="title">'. $form_title .'</span>';
  if ($conf['show_balance']) {
    //work out which currencies are in the form
    foreach ($form['#settings']['worth']['preset'] as $item) {
      $balance = theme(
        'worth_item',
        array(
          'currcode' => $item['currcode'],
          'quantity' => transaction_totals($GLOBALS['user']->uid, $item['currcode'])->balance
        )
      );
      $title .= ' <span class="balance">'. $balance .'</span>';
    }
  }
  return (object)array(
    'title' => $title,
    'content' => $form
  );
}
