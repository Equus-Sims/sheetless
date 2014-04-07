<script type="text/javascript" src="http://www.google.com/jsapi"></script>
<script type="text/javascript">
  google.load('visualization', '1', {packages:['gauge']});</script>
<?php
//$Id: balance_ometer.tpl.php,v 1.3 2010/12/08 11:43:18 matslats Exp $
$danger_zone_percent = 25;
$balance_gauge_pixels = 120;

/*
 * Balance_limits.tpl.php
 * Themed display the user's balance & limits for a given non-acknowledgement currency
 * variables:
 * $currcode
 * $max
 * $min
 * $uid
 * $balance
 */
$range = $max-$min;
$zone_size = $range * $danger_zone_percent / 200;

$min_safe = $min + $zone_size;
$max_safe = $max - $zone_size;
$id = $currcode.'-ometer-'.$uid;
?>
<script type="text/javascript">
function drawGauge() {
  var data = google.visualization.arrayToDataTable([
    ['Label', 'Value'],
    ['<?php print str_replace('[quantity]', '', strip_tags(currency_load($currcode)->display['format'])); ?>', <?php print $balance; ?>]
  ]);

  var options = {
    width: <?php print $balance_gauge_pixels; ?>,
    height: <?php print $balance_gauge_pixels; ?>,
    min: <?php print $min; ?>,
    max: <?php print $max; ?>,
    greenColor: '#FF9900', greenFrom: <?php print $min; ?>, greenTo: <?php print $min_safe; ?>,
    yellowColor: '#ffffff', yellowFrom:<?php print $min_safe; ?>, yellowTo: <?php print $max_safe; ?>,
    redColor: '#FF9900', redFrom: <?php print $max_safe; ?>, redTo: <?php print $max; ?>,
  };
  new google.visualization.Gauge(document.getElementById('<?php print $id; ?>')).draw(data, options);
}
google.setOnLoadCallback(drawGauge);
</script>
<div id ="<?php print $id; ?>" class = "limits-gauge" style ="width:<?php print $balance_gauge_pixels; ?>px; height:<?php print $balance_gauge_pixels; ?>px"></div>
