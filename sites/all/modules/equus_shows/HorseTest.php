<?php
 
require("equus_shows_horse.inc");

class HorseTest extends PHPUnit_Framework_TestCase {

    public function testHorseConstructor() {
        $string = '{
            "type": "Horse",
            "name": "Foo",
            "stats": {
                "action": 5,
                "balance": 8,
                "impulsion": 11,
                "responsiveness": 7,
                "rhythm": 10,
                "suppleness": 6
            }
        }';

        $the_horse = new equus_shows_horse(json_decode($string));

        $this->assertEquals("Foo", $the_horse->get_name());

        $this->assertEquals(5, $the_horse->get_stat("action"));

        $this->assertEquals(10, $the_horse->get_stat("rhythm"));

        $this->assertEquals(0, $the_horse->get_stat("speed"));
    }
}

?>