declare namespace $ds {

    interface TextBox {
        type: 'TextBox';
        label: string;
        default?: string;
    }
    
    interface TextArea {
        type: 'TextArea';
        label: string;
        default?: string
    }
    
    interface Slider {
        type: 'Slider';
        label: string;
        precision?: number;
        min?: number;
        max?: number;
        default?: number;
    }
    
    interface KeyNameBox {
        type: 'KeyNameBox';
        label: string;
        min?: number;
        max?: number;
        default?: number;
    }
    
    interface TimeBox {
        type: 'TimeBox';
        label: string;
        format: 'MBT'|'HMS';
        relative?: boolean;
        default?: number;
    }
    
    interface CheckBox {
        type: 'CheckBox';
        label: string;
        default?: boolean;
    }
    
    interface Select {
        type: 'Select';
        label: string;
        options: string[];
        default?: number;
    }
    
    interface Label {
        type: 'Label';
        label: string;
    }
    
    type FormWidget = TextBox | TextArea | Slider | KeyNameBox | TimeBox | CheckBox | Select | Label;
    
    type FormReturnType<T> = T extends TextBox ? string:
                         T extends TextArea ? string:
                         T extends Slider ? number:
                         T extends KeyNameBox ? number:
                         T extends TimeBox ? number:
                         T extends CheckBox ? boolean:
                         T extends Select ? number:
                         T extends Label ? null:
                         never;
    
    interface FormDialog<T extends FormWidget[]> {
        title: string;
        widgets: T;
    }

    namespace dialogSystem {
        function alert(title: string, message: string): void;
        function confirm(title: string, message: string, defaultButton: 'Yes'|'No'): boolean;
        function prompt(title: string, message: string): string;
        function form<T extends $ds.FormWidget[]>(formDialog: $ds.FormDialog<T>): {
            result: 'Ok'|'Cancel';
            form: {[K in keyof T]: $ds.FormReturnType<T[K]>};
        };
    }

    namespace actionSystem {

    }

    namespace project {

    }

    namespace utils {

    }

    abstract class _ScriptBase {

        public tr(text: string): string;

        public abstract locale(lang: string, text:string): string | undefined | null;

        public abstract info(): {
            id: string;
            name: string;
            role: string | string[]
            requiredEditorVersion: string;
        };

    }

    abstract class Script extends _ScriptBase {

        public abstract main(): void;

    }

    abstract class ScriptSet extends _ScriptBase {

        public abstract main(index: number): void;

        public abstract info(): {
            id: string;
            name: string;
            role: string | string[]
            requiredEditorVersion: string;
            children: {id: string, name: string}[];
        };
    
    }

    function register(id: string, role: string|string[], script: $ds._ScriptBase): void;
}
