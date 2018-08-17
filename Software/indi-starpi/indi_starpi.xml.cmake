="1.0" encoding="UTF-8"?> <driversList> <devGroup group="Telescopes">
        <device label="StarPi">
		<driver name="StarPi">indi_starpi_telescope</driver>
            <port>@STARPI_PORT@</port>
            <host>@STARPI_HOST@</host>
                <version>@STARPI_VERSION_MAJOR@.@STARPI_VERSION_MINOR@</version>
	</device> </devGroup> </driversList>
