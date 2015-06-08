<?php
require("equus_shows_show.inc");

class ShowTest extends PHPUnit_Framework_TestCase {

    private $show_string;

    public function __construct() {
        $this->setShowString();
    }

    public function setShowString() {
        $file = file_get_contents("show.json");
        $this->show_string = json_decode($file);
    }

    public function testShowConstructor() {
        $the_show = new equus_shows_show($this->show_string);

        $horses = $the_show->getHorses();

        $this->assertEquals("Foo", $horses[0]->get_Name());

        $this->assertEquals("Bar", $horses[1]->get_Name());
    }

    public function testRunShow() {
        $new_show = new equus_shows_show($this->show_string);

        $output = $new_show->run_the_show();

        //$ref_output = json_decode('[{"placement":1,"id":2,"scores":{"1":10.58,"2":10.64}},{"placement":2,"id":3,"scores":{"1":10.42,"2":10.36}},{"placement":3,"id":1,"scores":{"1":10,"2":10}}]');

        //$this->assertEquals($ref_output, $output);
    }
}

?>