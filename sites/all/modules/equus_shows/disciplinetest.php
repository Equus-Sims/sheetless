<?php
 
require("equus_shows_discipline.inc");

class DisciplineTest extends PHPUnit_Framework_TestCase {

    public function testDisciplineConstructor() {
        $string = '{
            "type": "Gaited Dressage",
            "id": 1,
            "stats": {
                "action": 60,
                "rhythm": 40
            }
        }';

        $the_discipline = new equus_shows_discipline(json_decode($string));

        $this->assertNotNull($the_discipline);

        $stats = $the_discipline->get_stats();

        $this->assertEquals(60, $stats->action);

        $this->assertEquals(40, $stats->rhythm);
    }
}

?>