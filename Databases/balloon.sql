
CREATE TABLE `balloon` (
  `color` varchar(20) default NULL,
  `size` varchar(20) default NULL,
  `act` varchar(20) default NULL,
  `age` varchar(20) default NULL,
  `class` varchar(20) default NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/* Data for the `balloon` table  (Records 1 - 20) */

INSERT INTO `balloon` (`color`, `size`, `act`, `age`, `class`) VALUES 
  ('yellow', 'small', 'stretch', 'adult', 'true'),
  ('yellow', 'small', 'stretch', 'adult', 'true'),
  ('yellow', 'small', 'stretch', 'child', 'false'),
  ('yellow', 'small', 'dip', 'adult', 'false'),
  ('yellow', 'small', 'dip', 'child', 'false'),
  ('yellow', 'large', 'stretch', 'adult', 'true'),
  ('yellow', 'large', 'stretch', 'adult', 'true'),
  ('yellow', 'large', 'stretch', 'child', 'false'),
  ('yellow', 'large', 'dip', 'adult', 'false'),
  ('yellow', 'large', 'dip', 'child', 'false'),
  ('purple', 'small', 'stretch', 'adult', 'true'),
  ('purple', 'small', 'stretch', 'adult', 'true'),
  ('purple', 'small', 'stretch', 'child', 'false'),
  ('purple', 'small', 'dip', 'adult', 'false'),
  ('purple', 'small', 'dip', 'child', 'false'),
  ('purple', 'large', 'stretch', 'adult', 'true'),
  ('purple', 'large', 'stretch', 'adult', 'true'),
  ('purple', 'large', 'stretch', 'child', 'false'),
  ('purple', 'large', 'dip', 'adult', 'false'),
  ('purple', 'large', 'dip', 'child', 'false');


