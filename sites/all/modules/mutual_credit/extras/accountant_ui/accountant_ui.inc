<?php

/*
 * menu form callback
 * $type denontes which of the 2 mass payments requested, many2one, one2many
 */
function mcapi_mass_pay($form, &$form_state, $vector) {
  $router_item = menu_get_item();
  drupal_set_title($router_item['title']);
  form_load_include($form_state, 'inc', 'accountant_ui');
  form_load_include($form_state, 'admin.inc', 'mcapi');
  $form = array();
  list($payer, $payee) = explode('2', $vector);
  if (empty($form_state['confirmed'])) {

    field_attach_form('transaction', entity_create('transaction', array()), $form, $form_state);

    $one = array(
      '#description' => t('A username, email, or user ID'),
      '#type' => 'user_chooser_few',
      '#callback' => 'user_chooser_segment_perms',
      '#args' => array('transact'),
      '#weight' => 9,
      '#multiple' => FALSE,
    );
    $few = array(
      '#type' => 'user_chooser_few',
      '#callback' => 'user_chooser_segment_perms',
      '#args' => array('transact'),
      '#multiple' => TRUE,
      '#multiple_fail_quietly' => TRUE
    );
    $many = array(
      '#type' => 'user_chooser_many',
    );

    if ($payer == 'one') {
      $form['payers'] = array(
        '#title' => t('Payer'),
        '#weight' => 1
      ) + $one;
      $form['payees'] = array(
        '#title' => t('Payees'),
        '#weight' => 2
      )+ $$payee;
    }
    elseif ($payee == 'one') {
      $form['payees'] = array(
        '#title' => t('Payee'),
        '#weight' => 1
      ) + $one;
      $form['payers'] = array(
        '#title' => t('Payers'),
        '#weight' => 2
      )+ $$payer;
    }
    $form['mail'] = mcapi_mail_config_fieldset('mcapi_accounting_masspay', array('transactions'), FALSE, 21);
    $form['mail']['#title'] = $payer == 'one' ? t('Mail all payees (optional)') : t('Mail all payers (optional)');
    unset($form['mail']['cc']);
    $form['mail']['#description'] = t('Do not use payer and payee tokens because all will receive the same mail.');
    $form['mail']['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('transaction'),
      '#global_types' => FALSE,
      '#weight' => 3,
    );
  }
  else {
    $sentence = str_replace('[transaction:links]', '', variable_get('mcapi_sentence', mcapi_sentence_default()));
    //we have to preview these separately to see them all
    foreach ($form_state['confirmed'] as $transaction) {
      $form['preview'][] = transaction_view($transaction, $sentence);
    }
  }
  $form['buttons'] = array(
    '#weight' => 20,
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Record'),
      //this prevents double click, but doesn't prevent going back and resubmitting the form
      '#attributes' => array('onclick' => "this.disabled=true,this.form.submit();"),
      '#weight' => 20
    )
  );
  return $form;
}
/*
 * form validation callback
 * ensure that all the users have permission and test write all transactions
 */
function mcapi_mass_pay_validate($form, &$form_state) {
  if (!isset($form_state['confirmed'])) {
    $form_state['values']['creator'] = $GLOBALS['user']->uid;
    $form_state['values']['type'] = 'mass';
    $form_state['values']['state'] = TRANSACTION_STATE_FINISHED;
    $transactions = mcapi_mass_pay_insert($form_state, FALSE);
    if (form_get_errors()) return;
    $form_state['rebuild'] = TRUE;
    drupal_set_title(t('Are you sure?'));
    $form_state['storage'] = $form_state['values'];
    $form_state['confirmed'] = $transactions;//for displaying at the top of the confirmation page
  }
}
/*
 * form submit callback
 */
function mcapi_mass_pay_submit($form, &$form_state) {
    //store the mail so we can bring it up as a default next time
  variable_set('mcapi_accounting_masspay_mail', array(
    'subject' => $form_state['storage']['mcapi_accounting_masspay_subject'],
    'body' => $form_state['storage']['mcapi_accounting_masspay_body'],
  ));
  $params = variable_get('mcapi_accounting_masspay_mail');
  $form_state['values'] = &$form_state['storage'];
  form_state_values_clean($form_state);
  //the transactions are actually saved here
  $serial = mcapi_mass_pay_insert($form_state, TRUE);
  //now start preparing the mail
  if (strlen($params['subject']) && strlen($params['body'])) {
    $mailto = count($form_state['values']['payees']) > 1 ? 'payee' : 'payer';
    global $language;
    $xids = array_keys(transaction_filter(array('serial' => $serial)));
    //TODO batch this
    foreach (entity_load('transaction', $xids) as $transaction) {
      $params['transaction'] = $transaction;
      $account = user_load($transaction->$mailto);
      drupal_mail('accountant_ui', 'mass', $account->mail, user_preferred_language($account, $language), $params);
    }
  }
  $form_state['redirect'] = 'transaction/'.$serial;
}

/*
 * implements hook_mail
 */
function accountant_ui_mail($key, &$message, $params) {
  $options = array('language' => $message['language'], 'sanitize' => FALSE);
  $message['subject'] = token_replace($params['subject'], $params, $options);
  $message['body'][] = token_replace($params['body'], $params, $options);
}

function mcapi_mass_pay_insert($form_state, $really) {
  $values = $form_state['values'];
  unset($values['subject'], $values['body']);

  $payers = (array)$values['payers'];
  $payees = (array)$values['payees'];
  unset($values['payers'], $values['payees']);
  $template = entity_create('transaction', $values);
  entity_form_submit_build_entity('transaction', $template, $form_state['complete form'], $form_state);
  foreach ($payers as $payer) {
    foreach ($payees as $payee) {
      if ($payer == $payee) {
        drupal_set_message(t("Skipped transaction in which account '@account' pays itself", array('@account' => format_username(user_load($payee)))));
        continue;
      }
      $template->payer = $payer;
      $template->payee = $payee;
      $transactions[] = clone $template;
    }
  }
  try {
    $serial = transaction_cluster_write($transactions, $really);
  }
  catch (Exception $e) {
    form_set_error('', $e->getMessage());
  }
  if ($really) return $serial;
  else return $transactions;
}

/*
 * trading_groups callback declared in hook_user_chooser_preselect
 * return all the users with permission to trade
 * needs no arguments
 */
function accounting_list_all_users($element, $form_state) {
  $roles = user_roles(FALSE, 'transact');
  $query = db_select('users', 'u')
    ->fields('u', array('uid'))
    ->condition('status', TRUE);
  if (empty($roles[DRUPAL_AUTHENTICATED_RID])) {
    $query->join('users_roles', 'ur', 'ur.uid = u.uid');
    //if we chain this it breaks with: Call to a member function condition() on a non-object
    $query->condition('ur.rid', array_keys($roles));
  }
  return $query->execute()->fetchCol();
}
