CREATE TABLE `testing` (
  `refund` varchar(20) default NULL,
  `marst` varchar(20) default NULL,
  `taxinc` varchar(20) default NULL,
  `class` varchar(20) default NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/* Data for the `testing` table  (Records 1 - 10) */

INSERT INTO `testing` (`refund`, `marst`, `taxinc`, `class`) VALUES 
  ('yes', 'single', 'GT80', 'no'),
  ('no', 'married', 'GT80', 'no'),
  ('no', 'single', 'LT80', 'no'),
  ('yes', 'married', 'GT80', 'no'),
  ('no', 'divorced', 'GT80', 'yes'),
  ('no', 'married', 'LT80', 'no'),
  ('yes', 'divorced', 'GT80', 'no'),
  ('no', 'single', 'GT80', 'yes'),
  ('no', 'married', 'LT80', 'no'),
  ('no', 'single', 'GT80', 'no');


