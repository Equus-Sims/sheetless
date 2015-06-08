<?php
 
require("equus_shows_judge.inc");

class JudgeTest extends PHPUnit_Framework_TestCase {

    public function testJudgeConstructor() {
        $string = '{
            "type": "Judge",
            "name":"Bob",
            "stat_preferences": {
                "action": -2,
                "rhythm": 2
            }
        }';

        $the_judge = new equus_shows_judge(json_decode($string));

        $this->assertEquals("Bob", $the_judge->get_name());

        $this->assertEquals(-2, $the_judge->get_stat_preference("action"));

        $this->assertEquals(2, $the_judge->get_stat_preference("rhythm"));

        $this->assertEquals(0, $the_judge->get_stat_preference("speed"));
    }
}
 
?>