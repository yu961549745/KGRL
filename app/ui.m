function varargout = ui(varargin)
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @ui_OpeningFcn, ...
                   'gui_OutputFcn',  @ui_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end
if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
function ui_OpeningFcn(hObject, eventdata, handles, varargin)
handles.output = hObject;
guidata(hObject, handles);
function varargout = ui_OutputFcn(hObject, eventdata, handles) 
varargout{1} = handles.output;
function dataset_Callback(hObject, eventdata, handles)
function dataset_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function train_Callback(hObject, eventdata, handles)
global tasks
fh=getfield(tasks,[getData(handles),'_Train']);
fh();
function test_Callback(hObject, eventdata, handles)
global tasks
fh=getfield(tasks,[getData(handles),'_Test']);
fh();
function FB_A_Callback(hObject, eventdata, handles)
function FB_B_Callback(hObject, eventdata, handles)
function WN_A_Callback(hObject, eventdata, handles)
function WN_B_Callback(hObject, eventdata, handles)
function data = getData(handles)
data=handles.dataset.SelectedObject.String;
