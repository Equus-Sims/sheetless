<?php 
require("equus_shows_horse.inc");
require("equus_shows_discipline.inc");
require("equus_shows_judge.inc");

class equus_shows_show {
	private $horses;

	private $judges;

	private $discipline;

	public function __construct($data) {
		$horse_list = $data->horses;

		foreach ($horse_list as $horse) {
			$this->horses[] = new equus_shows_horse($horse);
		}

		$judge_list = $data->judges;

		foreach ($judge_list as $judge) {
			$this->judges[] = new equus_shows_judge($judge);
		}

		$this->discipline = new equus_shows_discipline($data->discipline);
	}

	public function getHorses() {
		return $this->horses;
	}

	public function run_the_show() {
		$entries = [];
		foreach($this->horses as $horse) {
			$entry = new stdClass();
			$entry->horse_id = $horse->get_id();
			$entry->scores = [];
			
			
			foreach($this->judges as $judge) {
				$score = 0;
				foreach($this->discipline->get_stats() as $stat => $weight) {
					$score += ($horse->get_stat($stat) / 100) * ($weight + $judge->get_stat_preference($stat));
				}
				
				$entry->scores[$judge->get_id()] = $score;
			}



			$entry->final_value = array_sum($entry->scores) / count($entry->scores);

			$entries[] = $entry;
		}
		

		// sort horse array by action attribute
		usort($entries, function($a, $b) {
			return $a->final_value < $b->final_value;
		});

		// Build the output array
		$output = [];

		for ($i = 0; $i < count($entries); $i++) {
			$temp = new stdClass();
			$temp->placement = $i + 1;
			$temp->id = $entries[$i]->horse_id;
			$temp->scores = (object)$entries[$i]->scores;
			$output[] = $temp;
		}

		return $output;
	}
}

?>
