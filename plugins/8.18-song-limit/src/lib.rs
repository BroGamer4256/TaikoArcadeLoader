#![allow(non_snake_case)]
use libc::*;
use std::arch::*;
use winapi::um::libloaderapi::*;

const SONG_DATA_SIZE: usize = 1024 * 1024 * 64;
static mut SONG_DATA: Option<*mut c_void> = None;
static mut HANDLE: Option<usize> = None;

pub unsafe fn gen_jmp(to: usize) -> Box<[u8]> {
	// MOV RAX, {TO}
	// JMP RAX
	Box::new([
		0x48,
		0xB8,
		to as u8,
		(to >> 8) as u8,
		(to >> 16) as u8,
		(to >> 24) as u8,
		(to >> 32) as u8,
		(to >> 40) as u8,
		(to >> 48) as u8,
		(to >> 56) as u8,
		0xFF,
		0xE0,
	])
}

macro_rules! primitive_ptr {
	($function:expr) => {
		$function as *mut ()
	};
}

macro_rules! set_crown_data {
	($offset:literal, $value:literal) => {
		asm!(
			"add rsp, 0x28",
			"mov rax, qword ptr [rsp + 0x40]",
			"add rax, r14",
			"lea rcx, [rax + rax * 8]",
			concat!("cmp dword ptr [rdx + rcx * 8 + 0x300], ", $value),
			"jge 1f",
			concat!("mov dword ptr [rdx + rcx * 8 + 0x300], ", $value),
			"1:",
			in("rdx") SONG_DATA.unwrap(),
		);
		asm!(
			"jmp rcx",
			in("rcx") HANDLE.unwrap() + $offset,
		)
	};
}

pub unsafe fn set_crown_data_1() {
	set_crown_data!(0x2F3AF1, 1);
}

pub unsafe fn set_crown_data_2() {
	set_crown_data!(0x2F3A27, 2);
}

pub unsafe fn set_crown_data_3() {
	set_crown_data!(0x2F3C25, 3);
}

macro_rules! set_score_rank {
	($offset:literal, $value:literal) => {
		asm!(
			"add rsp, 0x28",
			"lea rcx, [0xB + RBX * 4]",
			"add rcx, rsi",
			"add rcx, rbx",
			"lea rax, [rcx + rcx * 8]",
			concat!("cmp dword ptr [rdx + rax * 8], ", $value),
			"jge 1f",
			concat!("mov dword ptr [rdx + rax * 8], ", $value),
			"1:",
			in("rdx") SONG_DATA.unwrap(),
		);
		asm!(
			"jmp rcx",
			in("rcx") HANDLE.unwrap() + $offset,
		);
	};
}

pub unsafe fn set_score_rank_1() {
	set_score_rank!(0x306613, 1);
}

pub unsafe fn set_score_rank_2() {
	set_score_rank!(0x306547, 2);
}

pub unsafe fn set_score_rank_3() {
	set_score_rank!(0x30647B, 3);
}

pub unsafe fn set_score_rank_4() {
	set_score_rank!(0x3068D3, 4);
}

pub unsafe fn set_score_rank_5() {
	set_score_rank!(0x306807, 5);
}

pub unsafe fn set_score_rank_6() {
	set_score_rank!(0x30673B, 6);
}

pub unsafe fn set_score_rank_7() {
	set_score_rank!(0x3069D0, 7);
}

pub unsafe fn set_unknown_data_1() {
	asm!(
		"add rsp, 0x28",
		"lea rcx, [rsi + rsi * 4]",
		"add rcx, rdi",
		"lea rax, [rcx + rcx * 8]",
		"jmp r15",
		in("rdx") SONG_DATA.unwrap(),
		in("r15") HANDLE.unwrap() + 0x31376A,
	);
}

pub unsafe fn set_unknown_data_2() {
	asm!(
		"add rsp, 0x28",
		"lea rcx, [rdi + rdi * 4]",
		"add rcx, rbx",
		"lea rax, [rcx + rcx * 8]",
		"jmp rsi",
		in("rdx") SONG_DATA.unwrap(),
		in("rsi") HANDLE.unwrap() + 0x313A20,
	)
}

pub unsafe fn set_unknown_data_3() {
	asm!(
		"add rsp, 0x28",
		"lea rcx, [rdi + rdi * 4]",
		"add rcx, rbx",
		"lea rax, [rcx + rcx * 8]",
		"jmp r10",
		in("rdx") SONG_DATA.unwrap(),
		in("r10") HANDLE.unwrap() + 0x313B61,
	);
}

pub unsafe fn set_unknown_data_4() {
	asm!(
		"add rsp, 0x28",
		"lea rdx, [rdi + rdi * 4]",
		"add rdx, rbx",
		"lea rdx, [rdx + rdx * 8]",
		"jmp rax",
		in("r8") SONG_DATA.unwrap(),
		in("rax") HANDLE.unwrap() + 0x313C57,
	);
}

pub unsafe fn set_unknown_data_other() {
	asm!(
		"add rsp, 0x28",
		"lea rcx, [rdi + rdi * 4]",
		"lea rax, [rbx + 0xB]",
		"add rcx, rax",
		"lea rax, [rcx + rcx * 8]",
		"jmp r10",
		in("rdx") SONG_DATA.unwrap(),
		in("r10") HANDLE.unwrap() + 0x313D51,
	);
}

pub unsafe fn set_crown_data_other() {
	asm!(
		"add rsp, 0x28",
		"lea rdx, [rdi + rdi * 4]",
		"add rdx, rbx",
		"lea rax, [rdx + rdx * 8]",
		"jmp rcx",
		in("r8") SONG_DATA.unwrap(),
		in("rcx") HANDLE.unwrap() + 0x3140EC,
	);
}

pub unsafe fn set_song_data() {
	asm!(
		"add rsp, 0x28",
		"movsxd rax, edi",
		"lea rdx, [rax + rax * 4]",
		"movsxd rdx, ebx",
		"add rdx, rax",
		"lea rax, [rdx + rdx * 8]",
		"lea rbx, [r8 + rax * 8]",
		"jmp rcx",
		in("r8") SONG_DATA.unwrap(),
		in("rcx") HANDLE.unwrap() + 0x31369A,
	);
}

#[no_mangle]
pub unsafe extern "C" fn Init() {
	let handle = GetModuleHandleA(0 as *const i8) as usize;
	HANDLE = Some(handle);
	SONG_DATA = Some(calloc(1, SONG_DATA_SIZE));

	let bytes = gen_jmp(primitive_ptr!(set_crown_data_1) as usize);
	write_bytes(primitive_ptr!(handle + 0x2F3AC8), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_crown_data_2) as usize);
	write_bytes(primitive_ptr!(handle + 0x2F39FE), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_crown_data_3) as usize);
	write_bytes(primitive_ptr!(handle + 0x2F3BFC), bytes);

	let bytes = gen_jmp(primitive_ptr!(set_score_rank_1) as usize);
	write_bytes(primitive_ptr!(handle + 0x3065EC), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_score_rank_2) as usize);
	write_bytes(primitive_ptr!(handle + 0x306520), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_score_rank_3) as usize);
	write_bytes(primitive_ptr!(handle + 0x306454), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_score_rank_4) as usize);
	write_bytes(primitive_ptr!(handle + 0x3068AC), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_score_rank_5) as usize);
	write_bytes(primitive_ptr!(handle + 0x3067E0), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_score_rank_6) as usize);
	write_bytes(primitive_ptr!(handle + 0x306714), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_score_rank_7) as usize);
	write_bytes(primitive_ptr!(handle + 0x3069A4), bytes);

	let bytes = gen_jmp(primitive_ptr!(set_unknown_data_1) as usize);
	write_bytes(primitive_ptr!(handle + 0x313757), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_unknown_data_2) as usize);
	write_bytes(primitive_ptr!(handle + 0x313A0D), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_unknown_data_3) as usize);
	write_bytes(primitive_ptr!(handle + 0x313B4E), bytes);
	let bytes = gen_jmp(primitive_ptr!(set_unknown_data_4) as usize);
	write_bytes(primitive_ptr!(handle + 0x313C44), bytes);

	let bytes = gen_jmp(primitive_ptr!(set_unknown_data_other) as usize);
	write_bytes(primitive_ptr!(handle + 0x313D3A), bytes);

	let bytes = gen_jmp(primitive_ptr!(set_crown_data_other) as usize);
	write_bytes(primitive_ptr!(handle + 0x3140D9), bytes);

	let bytes = gen_jmp(primitive_ptr!(set_song_data) as usize);
	write_bytes(primitive_ptr!(handle + 0x31367D), bytes);
}

#[no_mangle]
pub unsafe extern "C" fn AfterCard1Insert() {
	memset(SONG_DATA.unwrap(), 0, SONG_DATA_SIZE);
}

#[no_mangle]
pub unsafe extern "C" fn AfterCard2Insert() {
	memset(SONG_DATA.unwrap(), 0, SONG_DATA_SIZE);
}

pub unsafe fn write_bytes<T>(location: *mut T, bytes: Box<[u8]>) -> Option<()> {
	region::protect(
		location,
		bytes.len(),
		region::Protection::READ_WRITE_EXECUTE,
	)
	.ok()?;
	memcpy(
		location as *mut libc::c_void,
		std::ptr::addr_of!(*bytes) as *mut c_void,
		bytes.len(),
	);
	Some(())
}
