<?php
/*
 * theme implementation of pending_signatures
 * show the $transaction->pending signatures, signed and unsigned, with links
 *
 * variables are
 * $transaction - original data pulled from transaction object
 *
 * css is calculated in hook_theme and included with
 *
 */
//inject a bit of css to change the background picture of the transaction certificate
$background =  "background-repeat: no-repeat; background-position: center; background-size: 32px";
$rows = array();
global $base_url;
$path = $base_url .'/'. drupal_get_path('module', 'mcapi_signatures');
foreach ($transaction->pending_signatures as $uid => $status) {
  if ($status == 1)  {
    $row = array(
      'title' => t('Awaiting signature'),
      'class' => 'pending',
      'style' => "background-image:url(\"$path/pending.png\"); width:32px; height:32px; $background"
    );
  }
  else {
    $row = array(
      'title' => t('Signed'),
      'class' => 'signed',
      'style' => "background-image:url(\"$path/finished.png\"); width:32px; height:32px; $background"
    );
  }

  $rows[$uid] = array(
    format_username(user_load($uid)),
    $row
  );
}
$table = array(
  '#theme' => 'table',
  '#attributes' => array('style' => "width:15em;"),
  '#rows' => $rows
);
?>
<div class ="pending-bgstamp"><?php print t('Pending'); ?></div>
<div id ="pending-signatures">
  <h2><?php print $transaction->state == TRANSACTION_STATE_FINISHED ? t('Signed by') : t('Awaiting Signatures'); ?></h2>
  <?php print render($table); ?>
</div>
