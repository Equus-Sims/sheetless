<?php

/* two different displays according to the currency type
 * $currcode is a machine name of a currency
 * $totals is an object with the following properties
 *   'balance' => float
 *   'gross_in' => float
 *   'gross_out' => float
 *   'volume' => float
 *   'count' => integer
 */

$currency = currency_load($currcode);
if ($totals->gross_in == 0 && $totals->gross_out== 0) return;
$id = "given-gotten-".$currcode;
?>
<script type="text/javascript" src="http://www.google.com/jsapi"></script>
<script type="text/javascript">
function drawGivenGotten() {
  var data = google.visualization.arrayToDataTable([
    ['', 'Given', 'Gotten'],
    ['', <?php print $totals->gross_in;?>, <?php print $totals->gross_out;?>],
  ]);
  var options = {
    title:"<?php print $currency->human_name .' '. t('Given / Gotten'); ?>",
    width:100,
    height:200,
    enableInteractivity: false,
    vAxis: {viewWindowMode: 'explicit'},
    vAxis: {viewWindow: {min: 0, max: <?php print 5 * ceil(max($totals->gross_in, $totals->gross_out) / 5); ?>}},
    hAxis: {title: "<?php print $totals->gross_in;?>, <?php print $totals->gross_out; ?>"},
    colors: ['#2aab49', '#21a0db'],
    legend: {position: 'none'}
  };
  new google.visualization.ColumnChart(document.getElementById('<?php print $id; ?>')).draw(data, options);
}
google.load('visualization', '1', {packages: ['corechart']});
google.setOnLoadCallback(drawGivenGotten);
</script>
<div id="<?php print $id; ?>" style="width:100px; height:200px;"></div>
