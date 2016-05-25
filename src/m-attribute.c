// This file is part of h5-memvol.
// 
// This program is is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with h5-memvol.  If not, see <http://www.gnu.org/licenses/>.



static void* create (void *obj, H5VL_loc_params_t loc_params, const char *attr_name, hid_t acpl_id, hid_t aapl_id, hid_t dxpl_id, void **req) {
	return NULL;
}

static void* open (void *obj, H5VL_loc_params_t loc_params, const char *attr_name, hid_t aapl_id, hid_t dxpl_id, void **req) {
	return NULL;
}

static herr_t read (void *attr, hid_t mem_type_id, void *buf, hid_t dxpl_id, void **req) {
	return 0;
}

static herr_t write (void *attr, hid_t mem_type_id, const void *buf, hid_t dxpl_id, void **req) {
	return 0;
}

static herr_t get (void *obj, H5VL_attr_get_t get_type, hid_t dxpl_id, void **req, va_list arguments) {
	return 0;
}

static herr_t specific (void *obj, H5VL_loc_params_t loc_params, H5VL_attr_specific_t specific_type, hid_t dxpl_id, void **req, va_list arguments) {
	return -1;
}

static herr_t optional (void *obj, hid_t dxpl_id, void **req, va_list arguments) {
	return -1;
}

static herr_t close (void *attr, hid_t dxpl_id, void **req) {
	return -1;
}
