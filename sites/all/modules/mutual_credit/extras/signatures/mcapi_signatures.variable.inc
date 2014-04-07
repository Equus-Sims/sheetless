<?php

/*
 * implements hook_variable
 */
function mcapi_signatures_variable() {
  $variables = array(
    'mcapi_sign_notify_template' => array(
      'title' => t('Transaction signature notification template'),
      'description' => t('A mail template (subject, body) for alerting users their signature is needed.'),
      'type' => 'array',
      'default' => array(
        'subject' => '[site:name]: '. t('Your signature required'),
        'body' => '[transaction:url:absolute]'
      )
    ),
  );
  $types = _get_signable_transaction_types();
  foreach ($types as $type) {
    $var_name = 'mcapi_signatures_'.$type;
    $variables[$var_name] = array(
      'title' => t('Signature settings for transaction type: @type', array('@type' => $var_name)),
      'description' => t('Specifies who should sign and who should be notified for transactions of this type'),
      'type' => 'array',
      'default' => array(
        'participants' => FALSE,
        'countersignatories' => array(),
        'notify' => FALSE
      )
    );
  }
}
