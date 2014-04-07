<?php
/*
 * preprocessor should do the theming here
 * balance_limits.tpl.php
 * $currency
 * $spend_limit
 * $earn_limit
 * $uid
 */
?>

<?php if ($spend_limit) print t('Spending limit:') . ' '. theme('worth_item', array('currcode' => $currcode, 'quantity' => $spend_limit)); ?>
<br />
<?php if ($earn_limit) print t('Receiving limit:') . ' '. theme('worth_item', array('currcode' => $currcode, 'quantity' => $earn_limit)); ?>

