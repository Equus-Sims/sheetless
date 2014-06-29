<?php

/**
 * @file
 * template.php
 */

function equus_bootstrap_subtheme_form_comment_form_alter(&$form, &$form_state, &$form_id) {
  $form['comment_body']['#after_build'][] = '_equus_bootstrap_subtheme_customize_comment_form';
}

function _equus_bootstrap_subtheme_customize_comment_form(&$form) {
  $form[LANGUAGE_NONE][0]['format']['#access'] = FALSE;
  return $form;
}

function equus_bootstrap_subtheme_preprocess_node(&$vars) {
	if ($vars['node']->type == "organization") {
		$ledger = equus_banking_retrieve_ledger();

		setlocale(LC_MONETARY, 'en_US');
		$vars['bank_balance'] = money_format('%.0n',equus_banking_balance($ledger, $vars['node']->nid));
		$vars['bank_transactions_path'] = "organization/transactions/{$vars['node']->nid}";
	}
}

function equus_bootstrap_subtheme_get_assoc_org($uid) {
	$query = new EntityFieldQuery();
	$query
		->entityCondition('entity_type', 'node')
		->entityCondition('bundle', 'organization')
		->propertyCondition('status', 1)
		->propertyCondition('uid', $uid);
	$result = $query->execute();
	$nid_owner = array();
	if (count($result) > 0) {
		$nid_owner = array_keys($result['node']);
	} 

	$query = new EntityFieldQuery();
	$query
		->entityCondition('entity_type', 'node')
		->entityCondition('bundle', 'organization')
		->propertyCondition('status', 1)
		->fieldCondition('field_co_owners', 'target_id', $uid);
	$result = $query->execute();
	$nid_co_owner = array();
	if (count($result) > 0) {
		$nid_co_owner = array_keys($result['node']);
	} 

	$nids = array_merge($nid_owner, $nid_co_owner);
	return $nids;
}

function equus_bootstrap_subtheme_get_net_worth($uid) {
	$ledger = equus_banking_retrieve_ledger();
	$nids = equus_bootstrap_subtheme_get_assoc_org($uid);
	
	$total = array_reduce($nids, function($carry, $item) use (&$ledger) {
		return $carry + equus_banking_balance($ledger, $item);
	}, 0);

	setlocale(LC_MONETARY, 'en_US');
	return money_format('%.0n', $total);
}

function equus_bootstrap_subtheme_preprocess_user_profile(&$vars) {
	$date = array_pop(field_get_items('user', $vars['elements']['#account'], 'field_user_dob'));
	$vars['age'] = (new DateTime($date['value']))->diff(new DateTime())->y;
	
	$vars['net_worth'] = equus_bootstrap_subtheme_get_net_worth($vars['elements']['#account']->uid);
}