-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Aug 17, 2023 at 04:36 PM
-- Server version: 10.4.22-MariaDB
-- PHP Version: 7.4.27

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `merpati`
--

-- --------------------------------------------------------

--
-- Table structure for table `burung`
--

CREATE TABLE `burung` (
  `id_burung` int(11) NOT NULL,
  `nama_burung` varchar(20) NOT NULL,
  `rfid` varchar(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `burung`
--

INSERT INTO `burung` (`id_burung`, `nama_burung`, `rfid`) VALUES
(1, 'Ardian', '111'),
(2, 'Yoga', '222'),
(3, 'Guetta', '124'),
(5, 'Miki', '8989');

-- --------------------------------------------------------

--
-- Table structure for table `datang`
--

CREATE TABLE `datang` (
  `id_datang` int(11) NOT NULL,
  `id_matchup` int(11) NOT NULL,
  `id_burung` int(11) NOT NULL,
  `waktu_datang` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `datang`
--

INSERT INTO `datang` (`id_datang`, `id_matchup`, `id_burung`, `waktu_datang`) VALUES
(1, 1, 1, '2023-06-22 19:00:00'),
(2, 1, 2, '2023-06-22 19:05:00'),
(3, 2, 1, '2023-06-23 09:05:00'),
(4, 2, 2, '2023-06-21 06:50:26'),
(5, 1, 3, '2023-06-22 21:00:00'),
(20, 15, 1, '2023-07-04 21:20:57'),
(21, 15, 2, '2023-07-05 16:01:27'),
(22, 15, 3, '2023-07-05 16:01:36'),
(23, 15, 5, '2023-07-05 16:01:42'),
(24, 17, 1, '2023-07-05 16:01:13'),
(25, 17, 2, '2023-07-05 16:01:27'),
(26, 17, 3, '2023-07-05 16:01:36'),
(27, 17, 5, '2023-07-05 16:01:42');

-- --------------------------------------------------------

--
-- Table structure for table `matchup`
--

CREATE TABLE `matchup` (
  `id_matchup` int(11) NOT NULL,
  `id_pertandingan` int(11) NOT NULL,
  `id_track` int(11) NOT NULL,
  `waktu_matchup` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `status` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `matchup`
--

INSERT INTO `matchup` (`id_matchup`, `id_pertandingan`, `id_track`, `waktu_matchup`, `status`) VALUES
(1, 1, 1, '2023-06-22 13:00:00', 1),
(4, 1, 2, '2023-06-30 17:00:00', 1),
(6, 11, 1, '2023-06-27 16:50:00', 1),
(8, 11, 2, '2023-07-03 11:26:30', 1),
(15, 2, 19, '2023-07-04 21:36:52', 1),
(16, 1, 19, '2023-07-05 13:47:00', 1),
(17, 14, 20, '2023-07-05 13:53:34', 1),
(18, 0, 21, '0000-00-00 00:00:00', 1),
(19, 0, 22, '0000-00-00 00:00:00', 1),
(20, 0, 23, '0000-00-00 00:00:00', 1);

-- --------------------------------------------------------

--
-- Table structure for table `pertandingan`
--

CREATE TABLE `pertandingan` (
  `id_pertandingan` int(11) NOT NULL,
  `nama_pertandingan` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `pertandingan`
--

INSERT INTO `pertandingan` (`id_pertandingan`, `nama_pertandingan`) VALUES
(1, 'Piala Walikota 1'),
(2, 'Magetan Cup'),
(11, 'Ponorogo Cup 2'),
(14, 'Surabaya Open 1'),
(15, 'asd');

-- --------------------------------------------------------

--
-- Table structure for table `track`
--

CREATE TABLE `track` (
  `id_track` int(11) NOT NULL,
  `nama_track` varchar(20) NOT NULL,
  `start_track` varchar(30) NOT NULL,
  `finish_track` varchar(30) NOT NULL,
  `panjang_track` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `track`
--

INSERT INTO `track` (`id_track`, `nama_track`, `start_track`, `finish_track`, `panjang_track`) VALUES
(1, 'Genengan', '-7.277170, 112.750045', '-7.279208, 112.790823', 36001),
(2, 'Ponorogo', '-7.278902, 112.790592', '-7.274635, 112.797853', 500),
(3, 'Dummy', '-7.280549, 112.780775', '-7.279208, 112.790823', 6000),
(18, 'Random Sukolilo 2', '-7.27717,112.750045', '-7.279208,112.790823', 0),
(19, 'Random Surabaya A', '-7.27717,112.750045', '-7.279208,112.790823', 4500),
(20, 'alun alun ponorogo', '-7.27717,112.750045', '-7.279208,112.790823', 1000),
(21, '', '-7.27717,112.750045', '-7.27717,112.750045', 0),
(22, '', '-7.27717,112.750045', '-7.27717,112.750045', 0),
(23, 'sentono', '-7.27717,112.750045', '-7.27717,112.750045', 0);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `burung`
--
ALTER TABLE `burung`
  ADD PRIMARY KEY (`id_burung`);

--
-- Indexes for table `datang`
--
ALTER TABLE `datang`
  ADD PRIMARY KEY (`id_datang`);

--
-- Indexes for table `matchup`
--
ALTER TABLE `matchup`
  ADD PRIMARY KEY (`id_matchup`);

--
-- Indexes for table `pertandingan`
--
ALTER TABLE `pertandingan`
  ADD PRIMARY KEY (`id_pertandingan`);

--
-- Indexes for table `track`
--
ALTER TABLE `track`
  ADD PRIMARY KEY (`id_track`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `burung`
--
ALTER TABLE `burung`
  MODIFY `id_burung` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT for table `datang`
--
ALTER TABLE `datang`
  MODIFY `id_datang` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=28;

--
-- AUTO_INCREMENT for table `matchup`
--
ALTER TABLE `matchup`
  MODIFY `id_matchup` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=21;

--
-- AUTO_INCREMENT for table `pertandingan`
--
ALTER TABLE `pertandingan`
  MODIFY `id_pertandingan` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=16;

--
-- AUTO_INCREMENT for table `track`
--
ALTER TABLE `track`
  MODIFY `id_track` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=24;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
