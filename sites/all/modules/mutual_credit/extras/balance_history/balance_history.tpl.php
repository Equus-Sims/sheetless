<script type="text/javascript" src="http://www.google.com/jsapi"></script>
<script type="text/javascript">
  google.load('visualization', '1', {packages: ['corechart']});</script>
<?php
////$Id: balance_history.tpl.php,v 1.3 2010/12/08 11:43:18 matslats Exp $

/*
 * Balance History Google Chart
 * Takes data in the format below and outputs an <img> tag for a google chart.
 * Feel free to tweak the initial variables
 * //TODO This could be cached.
 *
 * $account = User Obj
 * $histories = array(
 *   '$currcode' = array(
 *     '$unixtime' => $balance
 *     '$unixtime' => $balance
 *     etc...
 *   )
 * )
 * $width
 * $height
 *
 * https://developers.google.com/chart/interactive/docs/gallery/linechart
 */
$currcodes = array_keys($histories);
$color_sequence = array('21a0db', '2aab49');
foreach ($histories as $currcode => $history) {
  $colors[] = "'".array_pop($color_sequence) ."'";
  foreach ($history as $timestamp => $balance) {
    $timeline[$timestamp][$currcode] = $balance;
  }
}
if (empty($timeline))return '';
//$timeline is now a list of times and changes of balance in currencies
ksort($timeline);
//what we need is a list of times with both balances per moment
//starting with a default 'prev value
foreach (array_keys($histories) as $currcode) $prev[$currcode] = 0;
foreach ($timeline as $timestamp => $balances) {
  $vals = array_merge($prev, $balances);
  $timeline[$timestamp] = $vals;
  $prev = $timeline[$timestamp];
}

if (count($histories) == 1) $title = t('!currency history', array('!currency' => currency_load($currcode)->human_name));
else $title = t('Balance history');
$id = 'uid-'.$account->uid.'-'.implode('',array_keys($histories));?>
<script type="text/javascript">
function drawBalanceHistory() {
  var data = new google.visualization.DataTable();
  data.addColumn('date', 'Date');
<?php foreach (array_keys(current($timeline)) as $currcode) { ?>
  data.addColumn('number', "<?php print currency_load($currcode)->human_name; ?>");
<?php } ?>

<?php foreach ($timeline as $timestamp => $balances) {
  $date = 'new Date("'.date('m/d/Y', $timestamp).'")';?>
  data.addRow([<?php print $date; ?>, <?php print implode(', ', $balances);?>]);
<?php } ?>
  var options = {
    curveType: "<?php print $curvetype; ?>",
    width: <?php print $width; ?>,
    height: <?php print $height; ?>,
    colors: [<?php print implode(', ', $colors);?>],
    legend: {position: 'none'},
    title: "<?php print $title ?>"
  }
  new google.visualization.LineChart(document.getElementById('<?php print $id; ?>')).draw(data, options);
}
google.setOnLoadCallback(drawBalanceHistory);
</script>
<div id="<?php print $id;?>" class = "balance-history" style="width:<?php print $width; ?>px; height:<?php print $height; ?>px;"></div>
