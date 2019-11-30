CREATE TABLE `shuttleland` (
  `class` varchar(20) default NULL,
  `stability` varchar(20) default NULL,
  `error` varchar(20) default NULL,
  `sign` varchar(20) default NULL,
  `wind` varchar(20) default NULL,
  `magnitude` varchar(20) default NULL,
  `VISIBILITY` varchar(20) default NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/* Data for the `shuttleland` table  (Records 1 - 15) */

INSERT INTO `shuttleland` (`class`, `stability`, `error`, `sign`, `wind`, `magnitude`, `VISIBILITY`) VALUES 
  ('auto', '?', '?', '?', '?', '?', 'no'),
  ('manual', 'unstable', '?', '?', '?', '?', 'yes'),
  ('manual', 'stable', 'LX', '?', '?', '?', 'yes'),
  ('manual', 'stable', 'XL', '?', '?', '?', 'yes'),
  ('manual', 'stable', 'MM', 'NN', 'tail', '?', 'yes'),
  ('manual', '?', '?', '?', '?', 'outofrange', 'yes'),
  ('auto', 'stable', 'SS', '?', '?', 'low', 'yes'),
  ('auto', 'stable', 'SS', '?', '?', 'medium', 'yes'),
  ('auto', 'stable', 'SS', '?', '?', 'strong', 'yes'),
  ('auto', 'stable', 'MM', 'PP', 'head', 'low', 'yes'),
  ('auto', 'stable', 'MM', 'PP', 'head', 'medium', 'yes'),
  ('auto', 'stable', 'MM', 'PP', 'tail', 'low', 'yes'),
  ('auto', 'stable', 'MM', 'PP', 'tail', 'medium', 'yes'),
  ('manual', 'stable', 'MM', 'PP', 'head', 'strong', 'yes'),
  ('auto', 'stable', 'MM', 'PP', 'tail', 'strong', 'yes');


