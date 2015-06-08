<?php
 
class equus_shows_horse {
    private $name;
    private $id;
    private $stats;
 
    /// the caller must have already decoded json
    public function __construct($data) {
        $this->name = $data->name;
        $this->id = $data->id;

        // $stats will be null if the horse has no stats
        $this->stats = $data->stats;
    }

    public function get_name() {
        return $this->name;
    }

    public function get_id() {
        return $this->id;
    }

    public function get_stat($key) {
        if (isset($this->stats->$key)) {
            return $this->stats->$key;
        } else {
            return 0;
        }
    }
}
 
?>