<?php
 
class equus_shows_judge {
    private $name;
    private $id;
    private $stat_preferences;
 
    /// the caller must have already decoded json
    public function __construct($data) {
        $this->name = $data->name;
        $this->id = $data->id;

        // $stat_preferences will be null if the judge has no stat_preferences
        $this->stat_preferences = $data->stat_preferences;
    }

    public function get_name() {
        return $this->name;
    }

    public function get_id() {
        return $this->id;
    }

    public function get_stat_preference($key) {
        if (isset($this->stat_preferences->$key)) {
            return $this->stat_preferences->$key;
        } else {
            return 0;
        }
    }
}
 
?>