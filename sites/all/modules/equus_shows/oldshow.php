<?php
require("judge.php");
require("horse.php");

$string = file_get_contents("show.json");
$data = json_decode($string);

$judges = [];

$judge_list = $data->judges;
foreach ($judge_list as $judge) {
	$judges[] = new judge($judge);
}

$horses = [];

$horse_list = $data->horses;
foreach ($horse_list as $horse) {
	$horses[] = new horse($horse);
}

foreach ($horses as $horse) {
	$favorite_numbers = $horse->get_favorite_numbers();
	foreach ($judges as $judge) {
		$score = 3;
		foreach ($favorite_numbers as $number) {
			if ($judge->judge($number)){

			} else {
				$score--;
			}
		}
		echo "Judge {$judge->get_name()}: {$horse->get_name()}'s score is {$score}/3\n";
	}
}

?>