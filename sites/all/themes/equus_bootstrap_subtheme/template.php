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