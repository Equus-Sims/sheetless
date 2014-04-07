<?php

/*
 * implements hook_tokens for a transaction cluster
 * process one transaction at a time, containing dependents
 * also see entity_tokens and field_tokens
 */
function mcapi_tokens($type, $tokens, array $data = array(), array $options = array()) {
  if ($type != 'transaction' || !array_key_exists('transaction', $data) || !is_object($data['transaction'])) return array();
  module_load_include('inc', 'mcapi');
  $transaction = &$data[$type];
  $url_options = array('absolute' => TRUE);
  $sanitize = !empty($options['sanitize']);
  $language_code = NULL;

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  $replacements = array();
  foreach ($tokens as $name => $original) {
    switch ($name) {
      case 'serial':
        $replacements[$original] = $transaction->serial;
        break;
      case 'payer':
        $account = user_load($transaction->payer);
        $replacements[$original] = $sanitize ? filter_xss($account->name) : format_username($account);
        break;
      case 'payee':
        $account = user_load($transaction->payee);
        $replacements[$original] = $sanitize ? filter_xss($account->name) : format_username($account);
        break;
      case 'creator':
        $account = user_load($transaction->creator);
        $replacements[$original] = $sanitize ? filter_xss($account->name) : format_username($account);
        break;
      case 'created':
        $replacements[$original] = format_date($transaction->created, 'medium', '', NULL, $language_code);
        break;
      case 'certificate':
        $replacements[$original] = drupal_render(transaction_view($transaction, 'certificate'));
        break;
      case 'payer_diff':
      case 'payee_diff':
        $worth = $items = array();
        module_load_include('inc', 'mcapi');
        $diff = _transactions_diff($transaction);
        $uid = $name == 'payer_diff' ? $transaction->payer : $transaction->payer;
        foreach ($diff[$uid] as $currcode => $quantities) {
          $items[] = array('currcode' => $currcode, 'quantity' => array_sum($quantities));
        }
        $element = mcapi_field_formatter_view(NULL, NULL, NULL, NULL, NULL, $items);
        $replacements[$original] = drupal_render($element);
        break;
      case 'links':
        $ops = array();
        //the mcapi_forms module renders a transaction before it is saved with a serial numbmer
        if (property_exists($transaction, 'serial')) {
          $ops = transaction_get_links($transaction, FALSE, TRUE);
        }
        $replacements[$original] = drupal_render($ops);
        break;
      case 'url':
        $replacements[$original] = url('transaction/'.$transaction->serial, array('absolute' => TRUE));
        break;
      default: //hopefully a field API field
       // drupal_set_message("Unknown token $name");
    }
  }
  return $replacements;
}

function mcapi_token_info() {
  $info = array(
    'types' => array(
      'transaction' => array(
        'name' => t('Transaction'),
        'description' => t('Tokens related to a transaction cluster.'),
        'needs-data' => 'transaction',
      ),
    ),
    'tokens' => array(
      'transaction' => array(
        'serial' => array(
          'name' => t('Unique serial number'),
          'description' => t("The unique serial number of the transaction cluster."),
        ),
        'payer' => array(
          'name' => t('Payer name'),
          'description' => t("The user whose account is credited"),
        ),
        'payee' => array(
          'name' => t('Payee name'),
          'description' => t("The user whose account is debited"),
        ),
        'created' => array(
          'name' => t('Created'),
          'description' => t("When the transaction was created"),
        ),
        'creator' => array(
          'name' => t('Creator'),
          'description' => t("The user who first recorded the transaction"),
        ),
        'payer_diff' => array(
          'name' => t("Difference to payer in the main transaction"),
          'description' => t("Total change to payers balance from transaction cluster")
        ),
        'payee_diff' => array(
          'name' => t("Difference to payee in the main transaction"),
          'description' => t("Total change to payees balance from transaction cluster"),
        ),
        'certificate' => array(
          'name' => t('Certificate'),
          'description' => t("An HTML laid out view of the transaction cluster"),
        ),
        'links' => array(
          'name' => t('Links'),
          'description' => t("Links to transaction operations"),
        )
      )
    )
  );
  if (!module_exists('entity_token')) {//because there's a problem with it at the moment
    $info['tokens']['transaction']['url'] = array(
      'name' => t('Absolute url'),
      'description' => t("A link to the transaction's own page.")
    );
  }
  return $info;
}
/*
 * implements hook_token_info_alter
 * remove the 'original' set of tokens, which is added to all entities,
 * but is not necessary, because transactions are not edited
 */
function mcapi_token_info_alter(&$items) {
  unset($items['tokens']['transaction']['original']);
}
