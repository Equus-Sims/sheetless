<?php

class equus_shows_discipline {
	private $type;

	private $id;

	private $stats;
 
    /// the caller must have already decoded json
    public function __construct($data) {
        $this->type = $data->type;
        $this->id = $data->id;
        $this->stats = $data->stats;
    }

    public function get_stats() {
        return $this->stats;
    }
}

?>