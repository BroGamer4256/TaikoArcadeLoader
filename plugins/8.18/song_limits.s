.global set_song_data_rdx
.global set_song_data_r8

set_song_data_rdx:
	mov rdx, qword ptr song_data [rip]
	ret

set_song_data_r8:
	mov r8, qword ptr song_data [rip]
	ret
