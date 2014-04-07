<?php
// $Id: certificate.tpl.php,v 1.1.2.3 2010/12/06 13:19:46 matslats Exp $
/*
 * NOTE:
 *
 * Each currency can have its own transaction template
 * Simply rename this file and rename it thus in your theme directory:
 * transaction__$currcode.tpl.php where X is the currency id
 * (takes the first currency of any transaction)
 *
 * see template_preprocess_transaction() for details
 *
 * $object          //transction entity object
 * $id               //unique css identifer for this transaction
 * $type            //
 * $state           // 1 = pending, 0 = completed, -1 = erased
 * $recorded        // date formatted using drupal's 'medium' date format
 * $payer           // name linked to payer profile
 * $payee           // name linked to payee profile
 * $worth          // a comma separated list of formatted transaction values (in different currencies)
 * $children        //an array of other transactions with the same serial number
 * $links           //a list of links, ajax or normal, created using theme_links
 *
 * need to do some more work on the icon size
 * for now, you might want to include your own large-size graphic instead
 * using $transaction->quantity
 * tip: $currency = currency_load($transaction->currcode);
 *
 * If anyone can think of a more elegant way to make this translatable...
 */
$replacements = array(
  '@recorded' => $recorded,
  '!payer' => "\n".$payer,
  '!payee' => $payee."\n",
  '!worth' => '<span class = "quantity">'. $worth.'</span>'."\n",
);
if ($desc_fieldname = variable_get('transaction_description_field')) {
  $replacements['!description'] = render($additional[$desc_fieldname]);
}

$certificate_string = t('On @recorded !payer paid !payee the sum of !worth', $replacements);
$certificate_string = str_replace("\n", '<br /><br />', $certificate_string);
?>
<!--transaction.tpl.php-->
  <?php print render($pending); //floating on the right, by default ?>
  <?php print $certificate_string; ?>

  <?php if (array_key_exists('!description', $replacements)) : ?>
    <strong><?php print t('For:');?></strong>
    <?php print $replacements['!description'] ?><br />
  <?php endif; ?>

  <?php if (isset($dependents)) : // all the remaining transactions are already rendered as tokenised strings ?>
  <div id="dependent-transactions">
    <h3><?php print t('Dependent transactions'); ?></h3>
    <?php print render ($dependents); ?>
  </div>
  <?php endif; ?>

  <?php print render($additional); //any fields we don't know about'?>
  <?php print render($links); ?>
<!--/transaction.tpl.php-->

