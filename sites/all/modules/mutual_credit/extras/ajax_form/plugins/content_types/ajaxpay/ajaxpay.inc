<?php

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Ajax transaction form'),
  'description' => t('Create a (1st party transaction) in place'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('Form'),
	'defaults' => array(
		'direction' => 'outgoing',
	),
);

/**
 * Edit form.
 * No options for this yet
 */
function mcapi_ajaxform_ajaxpay_content_type_edit_form($form, &$form_state) {
	$conf = $form_state['conf'];

	$form['direction'] = array(
		'#title' => t('Direction'),
		'#type' => 'radios',
		'#options' => array(
		  'outgoing' => t('Current user pays'),
		  'incoming' => t('Current user requests transfer'),
		  'proposition' => t("This is a proposition node from the Offers & Wants module"),
		),
		'#default_value' => $conf->direction
	);
  return $form;
}
function mcapi_ajaxform_ajaxpay_content_type_edit_form_submit($form, &$form_state) {
	$form_state['conf']['direction'] = $form_state['values']['direction'];
}
/**
 * Render callback.
 */
function mcapi_ajaxform_ajaxpay_content_type_render($subtype, $conf, $args, $context) {
	if (!user_access('transact')) return;
  $node = &$context->data;
	if ($node->uid == $GLOBALS['user']->uid) return;
  if ($conf['direction'] == 'proposition') {
  	$direction = $node->want ? 'incoming' : 'outgoing';
  }
  else $direction = $conf['direction'];

  if ($direction == 'outgoing') {
    $props['payee'] = $node->uid;
    $props['payer'] = $GLOBALS['user']->uid;
  }
  else {
    $props['payer'] = $node->uid;
    $props['payee'] = $GLOBALS['user']->uid;
  }

  $props['type'] = '1stparty'; //new types dcan be declared in hook_mcapi_info_types
  $props['state'] = TRANSACTION_STATE_PENDING;//signatures module will alter state according to type, using hook_form_alter
  $props['currcode'] = 'credunit';//until we get better information
  $props['quantity'] = 1;//until we get better information
  $props['description'] = $node->title;
  //get the field api fields and put them on the transaction
  foreach (array_keys(field_info_instances('node', $node->type)) as $field_name) {
    $props[$field_name] = $node->{$field_name};
  }
  $transaction = entity_create('transaction', $props);

  $form = drupal_get_form('mcapi_ajaxform', $transaction);

  return (object) array(
    'title' => '<span class="title">' . t('Transfer') . '</span>',// <span class="balance">' . t('!balance', array('!balance' => $balance)). '</span>',
    'content' => $form
  );
}
